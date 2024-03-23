# Description
## How to Use
Change the STUDENT_ID inside main and run this command
``` bash
sudo bash run.sh
```

## First Receive of Message
If student id in server, type will be "201" and context is the key.
If student id not exist, type will be "401" and context is "-1".

## Second Receive of Message
After sending encode message to server, it will be return a packer.
And the type "203" for pass and  "403" for fail.