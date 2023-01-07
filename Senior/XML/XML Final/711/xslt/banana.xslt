<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
    <html>
      <head>
        <style>
          html, body {
              height: 100%;
              margin: 0;
            }
          body {
              display: flex; /*arrange object in order*/
              flex-direction: column; /*vertical arrangement*/
          }

          nav > ul {
              background-color: black;
              margin: 0;
              border-radius: 50px;
              padding: 0;
          }
          nav a {
              background-color: black;
              display: block; 
              font-size: 25px;
              padding: 15px;
          }
          .flex-nav {
              display: flex;
              justify-content: space-around ;
          }
          .flex-nav a{
              font-family: 'Comfortaa', cursive;

              display: block;
              float: left;
              color: white;
              padding: 5px;
              font-size: 25px;
              text-decoration: none;
          }

          .greet{
            display: flex;
            justify-content: center;
            align-items: center;
          }

          .wrapper {
              flex-grow: 1;
          }
          .wrapper p{
            font-family: 'Comfortaa', cursive;
            font-size: 18px;
            display: flex;
            justify-content: center;
            align-items: center;
          }
          .wrapper img{
            width: 25%;
            height: 50%;
            display:block; margin:auto;
          }

          .media{
            text-align: center;
          }
          .media img{
            width: 60;
            height: 60;
            /*mix-blend-mode: multiply;/*make white color to transparent*/
          }
          .media p{
            color: white;
          }

          .footer{
            background-color: black;
          }
        </style>
      </head>

      <body>
        <div class = "greet">
          <h2 align="center">哈囉哈囉 這裡是小竹輪喔喔喔</h2>
        </div>

        <div class="wrapper">
          <nav style="color: #b8d3ed; ">
            <ul class="flex-nav">
              <xsl:for-each select="data/nav">
                <td align="center">
                  <a>
                    <xsl:attribute name="href">
                      <xsl:value-of select="name" />
                    </xsl:attribute>

                    <h1 align="center">
                      <xsl:value-of select="title" />
                    </h1>
                  </a>
                </td>
              </xsl:for-each>
            </ul>
          </nav>

          <img src="../images/banana.jpg" alt="mssing page"></img>

          <table align="center" border="0">
            <h2 align="center">介紹</h2>
            <xsl:for-each select="data/introduction/detail">
              <tr>
                <td>
                  <xsl:value-of select="." />
                </td>
              </tr>
            </xsl:for-each>
          </table>
        </div>

        <footer class="footer">
          <div class="media">
            <a href="https://www.facebook.com/profile.php?id=100003522019305"><img src="../images/fb.png" alt="Welcome~"></img></a>
            <a href="https://github.com/yumeow0122"><img src="../images/github.png" alt="Welcome~"></img></a>
          </div>
        </footer>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>