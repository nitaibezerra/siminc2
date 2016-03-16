<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:acao_avaliacao.incC
   Finalidade: permitir o cadastro de avaliação
   */

$modulo=$_REQUEST['modulo'] ;//
if($_REQUEST['avp']){ $acaid = md5_decrypt($_REQUEST['avp'],'');}

if ($_POST['btgravar'])
{
   // fazer gravação de usuário na base de dados.
   $sql = "insert into usuario (usucpf,regcod,usunome,usuemail,usustatus, usufoneddd,usufonenum,usufuncao,orgcod,unicod) values (".
   "'".$_POST['USUCPF']."',".
   "'".$_POST['REGCOD']."',".
   "'".strtoupper($_POST['USUNOME'])."',".
   "'".$_POST['USUEMAIL']."',".
   "'A',".
   "'".$_POST['USUFONEDDD']."',".
   "'".$_POST['USUFONENUM']."',".
   "'".$_POST['USUFUNCAO']."',".
   "'".$_POST['ORGCOD']."',".
   "'".$_POST['UNICOD']."')";

  unset($POST);

   $saida = $db->executar($sql);
   if ($saida) $_SESSION['msgsistema'] = 'Operação realizada com sucesso' ; 
   else $_SESSION['msgsistema'] = 'Ocorreu falha na operação. Verifique os dados.';
}
?>
<html>

<head>

<meta name="GENERATOR" content="Microsoft FrontPage 4.0">
<meta name="ProgId" content="FrontPage.Editor.Document">
<title>Cadastro de Avaliações de Ação</title>
</head>

<body>


<div align="center">
<table  width='85%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;" align='center'>
  <tr align="center">
    <td width="100%"><strong>Cadastro de Avaliações de Ação</strong>
    &nbsp&nbsp&nbsp<span  title='Indica campo obrigatório'><img src="../imagens/obrig.gif"  align="middle"></span>...<a href="javascript:janela('ajuda_usuario.htm',700,500)"><img border="0" src="../imagens/help.gif"  alt='Ajuda desta página' title='Ajuda desta página'></a></td>
  </tr>
</table>
<table  align='center'  width='85%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
  <tr>
    <td width="100%"><font color=red><strong><?print $_SESSION['msgsistema']; $_SESSION['msgsistema'] = '' ?></strong></font></td>
  </tr>
</table>
<?
if($_POST["btatualizar"] and $_POST["USUCPF"] ) {
  // encontrar a avaliação para a ação específica e a referência específica.
        $sql= "select * from avaliacaoparecer where acaid=".$acaid;
        $saida = $db->recuperar($sql,$res);
        if(is_array($res)) {foreach($res as $k=>$v) ${$k}=$v[0];
        print '<input type=hidden name="acao" value="alterar">';	
        }
?>
<? } else { 


	$usunome = $_POST['usunome'];
	$usuemail =$_POST['usuemail'];
	$usufoneddd = $_POST['usufoneddd'];
	$usufonenum=$_POST['usufonenum'];
	$orgcod=$_POST['orgcod'];
	$usucpf = $_POST['usucpf'];
	$usufuncao = $_POST['usufuncao'];
	$unicod = $_POST['unicod'];
	$regcod = $_POST['regcod'];
?>
        <input type=hidden name="acao" value="inserir">
<? } ?>

<form method="POST" action="">
  <div align="center">
    <table border="0" width="85%">
      <tr>
        <td width="100%" colspan="2">
          <p align="center"><input type="button" value="Gravar" name="B1"><input type="button" value="Liberar" name="B2"></td>
      </tr>
      <center>
      <tr>
        <td width="50%"><select size="1" name="D1">
          </select>
          <p><input type="radio" value="V1" checked name="R1"><br>
          <input type="radio" name="R1" value="V2"><br>
          <input type="radio" name="R1" value="V3"></td>
        <td width="50%">
          <p align="center"><b>Avaliação</b></p>
          <p><textarea rows="3" name="S1" cols="40"></textarea></td>
      </tr>
      <tr>
        <td width="100%" colspan="2">
          <p align="center"><b>Avaliações anteriores ou paralelas</b></td>
      </tr>
      <tr>
        <td width="100%" colspan="2">
          <div align="center">
            <table border="0" width="85%">
              <tr>
                <td width="25%" align="center">
                  <p align="center"><b>Nome do avaliador e data</b></td>
                <td width="25%" align="center"><b>Texto da avaliação</b></td>
                <td width="25%" align="center"><b>Avalia</b></td>
                <td width="25%" align="center"><b>Avalia</b></td>
              </tr>
              <tr>
                <td width="25%"></td>
                <td width="25%"><textarea rows="3" name="S2" cols="40"></textarea></td>
                <td width="25%"></td>
                <td width="25%"></td>
              </tr>
            </table>
          </div>
        </td>
      </tr>
      <tr>
        <td width="100%" colspan="2">
          <p align="center"><input type="button" value="Gravar" name="B4"><input type="button" value="Liberar" name="B3"></td>
      </tr>
      </table>
    </center>
  </div>
</form>
<script>
    function atualizaComboUnidade(cod) {
	 //alert ("Deu certo"+cod);
        //document.formulario.orgao.value = cod;
	 
	 if (cod > 0) {document.formulario.submit()};
	 
    }


</script>
</body>

</html>