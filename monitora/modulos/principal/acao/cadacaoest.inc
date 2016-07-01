<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M�dulo:cadacaoest.inc
   Finalidade: permitir o controle do cadastro de acao
   */
$modulo=$_REQUEST['modulo'];

if ($_REQUEST['prgcod']) $_SESSION['prgcod'] = $_REQUEST['prgcod'];

include APPRAIZ."includes/cabecalho.inc";
if ($_REQUEST['acaid']) {
   $sql = "select * from acao where acaid=".md5_decrypt($_REQUEST['acaid'],'');
     $RS = $db->record_set($sql);
     $res = $db->carrega_registro($RS,0);
     if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
}
?>
<head>
<title>Estrutura de Ações do Simec</title>
</head>
<body>
<? 
abas(4,'monitora.php?modulo=principal/programa/listaprograma','Listar programa','monitora.php?modulo=principal/programa/cadprograma','Atributo',$_SESSION["sisdiretorio"].'.php?modulo=principal/programa/cadindicador&prgcod='.$_REQUEST['prgcod'],'Indicadores',$_SESSION['sisdiretorio'].".php?modulo=principal/acao/listaacao&prgcod=".$_REQUEST['prgcod'],'Ações',$_SESSION['sisdiretorio'].".php?modulo=principal/programa/monitoraprg&prgcod=".$_REQUEST['prgcod'],'Acompanhamento');
?>


<div align="center">
<table  width='100%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;" align='center'>
  <tr align="center">
    <td width="100%"><strong>Estrutura de ação</strong></td>
     </tr>
</table>

<table  align='center'  width='100%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
  <tr>
    <td width="100%"><font color=red><strong><? print $_SESSION['msgsistema']; $_SESSION['msgsistema'] = ''; ?></strong></font></td>
  </tr>
</table>
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="orgao" value=0>

    <table width='100%' align='center' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
      <tr>
        <td></td>
        <td width="25%">Código:</td>
        <td width="98%"><b><?=$acacod?></b>&nbsp;&nbsp;<?print $prgcod.'-'.$unicod.'('.$loccod.')'?>	
      </tr>
      <tr>
        <td></td>
        <td width="25%">Título:</td>
        <td width="98%">
	<textarea rows="2" name="acadsc" cols="70"><?=$acadsc?></textarea></td>
      </tr>
           
    </table>    
    </center>
<? 
abas(1,'monitora.php?modulo=principal/acao/listasubacao&acaid='.md5_encrypt($acaid,''),'Listar sub-ação','monitora.php?modulo=principal/acao/listaplantrab&acaid='.md5_encrypt($acaid,''),'Listar Plano de Trabalho','','','','','','');
?>

</form>

</td>
</tr>
</table>
</body>