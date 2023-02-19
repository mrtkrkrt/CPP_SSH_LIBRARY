#include "ssh.h"

ssh_session SSH::createSession()
{
    ssh_session session = ssh_new();

    ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.1.2");
    ssh_options_set(session, SSH_OPTIONS_USER, "Administrator");

    int returnCode = ssh_connect(session);

    if (returnCode != SSH_OK)
    {
        QString errorMessage =
            "Error connecting to host: " + QString::fromStdString(ssh_get_error(session));
        return nullptr;
    }

    returnCode = ssh_userauth_password(session, "Administrator", "uyssw");
    if (returnCode != SSH_OK)
    {
        QString errorMessage =
            "Error authenticating with password: " + QString::fromStdString(ssh_get_error(session));
        return nullptr;
    }

    return session;
}

int SSH::fileTransfer(ssh_session session, QString localFile, QString serverPath)
{
    int returnCode;

    if (session == nullptr) {
        QString errorMessage = "Session is null";
        return -1;
    }

    std::ifstream file(localFile.toStdString(), std::ios::binary);
    if (!file.is_open()) {
        QString errorMessage = "Error opening file";
        return -1;
    }

    file.seekg(0, file.end);
    size_t fileSize = file.tellg();
    file.seekg(0, file.beg);

    char* buffer = new char[fileSize];
    file.read(buffer, fileSize);

    sftp_session sftp = sftp_new(session);
    if (sftp == nullptr) {
        QString errorMessage =
            "Error allocating SFTP session: " + QString::fromStdString(ssh_get_error(session));
        return -1;
    }

    returnCode = sftp_init(sftp);
    if (returnCode != SSH_OK) {
        QString errorMessage =
            "Error initializing SFTP session: " + QString::fromStdString(ssh_get_error(session));
        return -1;
    }

    QString fileName = localFile.split("/").last();
    QString remoteFilePath = serverPath + "/" + fileName;

    sftp_file fileHandle =
        sftp_open(sftp, remoteFilePath.toLocal8Bit(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (fileHandle == nullptr) {
        QString errorMessage = "Error opening file on remote server: " +
                               QString::fromStdString(ssh_get_error(session));
        return -1;
    }

    returnCode = sftp_write(fileHandle, buffer, fileSize);
    if (returnCode < 0) {
        QString errorMessage = "Error writing file to remote server: " +
                               QString::fromStdString(ssh_get_error(session));
        return -1;
    }

    sftp_close(fileHandle);
    sftp_free(sftp);
    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}