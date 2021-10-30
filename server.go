// server
package main

import (
	//"C"
	//"flag"
	"flag"
	"fmt"
	//"log"
	"bufio"
	"net"
	"net/http"
	//"os/exec"
	"strings"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

var dir string = "/media/ming/disk1/MingDB/"

func parseRes(ori string) string {
	nstr := "<p>" + ori + "</p>"
	nstr = strings.Replace(nstr, "@", "</p><p>", -1)
	fmt.Println(nstr)
	return nstr
}

func Put(c echo.Context) error {
	data := c.FormValue("data")
	table := c.FormValue("table")
	key := c.FormValue("key")
	conn, _ := net.Dial("tcp", "127.0.0.1:8080")
	defer conn.Close()
	fmt.Fprintf(conn, "insert "+table+" "+key+" "+data)
	message, _ := bufio.NewReader(conn).ReadString('\n')
	fmt.Print("Message from server: " + message)

	return c.JSON(http.StatusOK, key)
}

func Update(c echo.Context) error {
	data := c.FormValue("data")
	table := c.FormValue("table")
	key := c.FormValue("key")
	conn, _ := net.Dial("tcp", "127.0.0.1:8080")
	defer conn.Close()
	fmt.Fprintf(conn, "update "+table+" "+key+" "+data)
	message, _ := bufio.NewReader(conn).ReadString('\n')
	fmt.Print("Message from server: " + message)
	return c.JSON(http.StatusOK, key)
}

func Delete(c echo.Context) error {
	table := c.FormValue("table")
	key := c.FormValue("key")
	conn, _ := net.Dial("tcp", "127.0.0.1:8080")
	defer conn.Close()
	fmt.Fprintf(conn, "delete "+table+" "+key)
	message, _ := bufio.NewReader(conn).ReadString('\n')
	fmt.Print("Message from server: " + message)
	return c.JSON(http.StatusOK, key)
}
func Get(c echo.Context) error {
	table := c.FormValue("table")
	key := c.FormValue("key")
	//ret := table + key
	conn, _ := net.Dial("tcp", "127.0.0.1:8080")
	defer conn.Close()
	fmt.Fprintf(conn, "get "+table+" "+key)
	ret, _ := bufio.NewReader(conn).ReadString('\n')
	fmt.Print("Message from server: " + ret)
	return c.JSON(http.StatusOK, parseRes(ret))
}
func Search(c echo.Context) error {
	table := c.FormValue("table")
	key := c.FormValue("key")
	num := c.FormValue("num")
	conn, _ := net.Dial("tcp", "127.0.0.1:8080")
	defer conn.Close()
	fmt.Fprintf(conn, "search "+table+" "+key+" "+num)
	message, _ := bufio.NewReader(conn).ReadString('\n')
	fmt.Print("Message from server: " + message)
	ret := parseRes(message)
	ret = strings.Replace(ret, key, "<em><font color=\"red\" >"+key+"</font></em>", -1)
	return c.JSON(http.StatusOK, ret)
}

var addr = flag.String("addr", "localhost:8080", "http service address")

func test(c echo.Context) error {
	conn, _ := net.Dial("tcp", "127.0.0.1:8080")
	defer conn.Close()
	fmt.Fprintf(conn, "test")
	message, _ := bufio.NewReader(conn).ReadString('\n')
	fmt.Print("Message from server: " + message)
	return nil
}

func main() {
	e := echo.New()
	fmt.Println("Echo run")
	e.Use(middleware.Logger())
	e.Use(middleware.Recover())
	e.Use(middleware.CORSWithConfig(middleware.CORSConfig{
		//AllowOrigins: []string{"http://www2.cs.ccu.edu.tw"},
		AllowOrigins: []string{"*"},
		AllowMethods: []string{echo.GET, echo.PUT, echo.POST, echo.DELETE, echo.OPTIONS},
		AllowHeaders: []string{echo.HeaderOrigin, echo.HeaderContentType, echo.HeaderAccept},
	}))

	e.POST("/put", Put)
	e.POST("/update", Update)
	e.POST("/delete", Delete)
	e.POST("/get", Get)
	e.POST("/search", Search)
	e.GET("/test", test)
	e.Logger.Fatal(e.Start(":1323"))
}
