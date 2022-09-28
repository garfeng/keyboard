package main

//#include "keyboard.h"
import "C"
import (
	"fmt"
	"log"

	"github.com/lxn/walk"
)

var (
	keyNumberPerSecond = 10
)

func main(){
	mw, err := walk.NewMainWindow()
	if err != nil {
		log.Fatal(err)
	}



	icon, err :=  walk.Resources.Icon("icon.ico") //walk.NewIconFromResource("icon.ico")
	if err != nil {
		log.Fatal(err)
	}

	ni, err := walk.NewNotifyIcon(mw)
	if err != nil {
		log.Fatal(err)
	}

	defer ni.Dispose()

	if err := ni.SetIcon(icon);err != nil {
		log.Fatal(err)
	}

	//ni.SetToolTip("右键选择不同击键或退出")

	for i:=10;i<=20;i+=5 {
		action := walk.NewAction()
		action.SetText(fmt.Sprintf("击键%d", i))
		action.Triggered().Attach(SetKeyTimesFunc(i))
		ni.ContextMenu().Actions().Add(action)
	}

	// We put an exit action into the context menu.
	exitAction := walk.NewAction()
	if err := exitAction.SetText("E&xit"); err != nil {
		log.Fatal(err)
	}
	exitAction.Triggered().Attach(func() { walk.App().Exit(0) })
	if err := ni.ContextMenu().Actions().Add(exitAction); err != nil {
		log.Fatal(err)
	}

	ni.SetVisible(true)

	if err := ni.ShowInfo("右键图标选择", "右键图标选择击键（默认10）或退出"); err != nil {
		log.Fatal(err)
	}

	go C.Setup()

	// Run the message loop.
	mw.Run()
}

func SetKeyTimesFunc(i int) func(){
	return func(){
			keyNumberPerSecond = i
			C.UpdateKeyPressRate(C.int(keyNumberPerSecond));
	}
}