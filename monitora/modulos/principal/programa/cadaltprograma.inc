<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:cadprograma.inc
   Finalidade: permitir o controle do cadastro de programas
   */

$modulo=$_REQUEST['modulo'] ;//

?>
<title>Cadastro de Programas do Simec</title>

<body>


<div align="center">
<table  width='85%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;" align='center'>
  <tr align="center">
    <td width="100%"><strong>Cadastro de Programas</strong>
    &nbsp&nbsp&nbsp<span  title='Indica campo obrigatório'><img src="../imagens/obrig.gif"  align="middle"></span>...<a href="javascript:janela('ajuda_usuario.htm',700,500)"><img border="0" src="../imagens/help.gif"  alt='Ajuda desta página' title='Ajuda desta página'></a></td>
  </tr>
</table>
<table  align='center'  width='85%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
  <tr>
    <td width="100%"><font color=red><strong><?print $_SESSION['msgsistema']; $_SESSION['msgsistema'] = '' ?></strong></font></td>
  </tr>
</table>
<?

$sql= "select * from programa where prgano = '".$_SESSION['exercicio']."'  order by prgcod";
if ($_POST['procura']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$_POST['procura']."'  order by prgcod";
if ($_REQUEST['prgcod']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$_REQUEST['prgcod']."'  order by prgcod";

$RS = $db->record_set($sql);

$nlinhas = $db->conta_linhas($RS);

// as linhas abaixo não permitem navegar para além dos limites
controla_navegacao($nlinhas);

// fim do controle de navegação

$res =  $db->carrega_registro($RS,$_POST['registro']);

// a linha abaixo transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	// encontrar o desafio (macro-objetivo)
    $sql= "select mobdsc from macroobjetivo where mobcod='".$mobcod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        //print '<input type=hidden name="acao" value="alterar">'        
?>

<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="orgao" value=0>
<input type=hidden name="registro" value=<?=$_POST['registro'];?>>
<input type=hidden name="navega" value=0>
<input type=hidden name="procura" value=0>
<? // inclusão do menu de setas de navegação
include "includes/setas_navegacao.inc";?> 

    <center>
    <table width='85%' align='center' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
      <tr>
        <td></td>
        <td width="25%">Código:</td>
        <td width="98%"><input type="text" name="prgcod" size="5" value='<?=$prgcod?>'>
	<img border="0" src="../imagens/busca.gif"  alt='Procura programa' title='Procura programa' onclick="ProcuraPrograma(this.form)"> 	
	<td></td>
      </tr>
      <tr>
        <td></td>
        <td width="25%">Orgão responsável:</td>
        <td width="98%">
	<?
	  $sql = "select orgcod as CODIGO,orgdsc as DESCRICAO from orgao order by orgdsc ";
	  $db->monta_combo("orgcod",$sql,"Selecione o órgão responsável",'','');
	?>
	</td>
	<td></td>
      </tr>
      <tr>
        <td></td>
        <td width="25%">Unidade responsável:</td>
         <td width="98%">
	<?
	  if ($orgcod) {
	  $sql = "select unicod as CODIGO,unidsc as DESCRICAO from unidade where orgcod ='".$orgcod."' order by unidsc ";
	  $db->monta_combo('unicod',$sql,'Selecione a Unidade','','');
	  }
	?>
	</td>
	<td></td>
      </tr>
      <tr>
        <td></td>
        <td width="25%">Denominação:</td>
        <td width="98%"><input type="text" name="prgdsc" size="60" value='<?=$prgdsc?>'></td>
	<td></td>
      </tr>

      <tr>
        <td width="1%"></td>
        <td width="25%">Problema:</td>
        <td width="98%">
	<textarea rows="2" name="prgdscproblema" cols="50"><?=$prgdscproblema?></textarea></td>
	<td></td>
      </tr>

      <tr>
        <td width="1%"></td>
        <td width="25%">Objetivo do Programa:</td>
        <td width="98%">
	<textarea rows="2" name="prgdscobjetivo" cols="50"><?=$prgdscobjetivo?></textarea></td>
	<td></td>
      </tr>
      <tr>
        <td width="1%"></td>
        <td width="25%">Público-Alvo:</td>
        <td width="98%">
	<textarea rows="2" name="prgdscpublicoalvo" cols="50"><?=$prgdscpublicoalvo?></textarea></td>
	<td></td>
      </tr>
      <tr>
        <td width="1%"></td>
        <td width="25%">Justificativa:</td>
        <td width="98%">
	<textarea rows="2" name="prgdscjustificativa" cols="50"><?=$prgdscjustificativa?></textarea></td>
	<td></td>
      </tr>
      <tr>
        <td width="1%"></td>
        <td width="25%">Objetivo setorial associado:</td>
        <td width="98%">
	<textarea rows="2" name="prgdscobjetivoassoc" cols="50"><?=$prgdscobjetivoassoc?></textarea></td>
	<td></td>
      </tr>
      <tr>
        <td width="1%"></td>
        <td width="25%">Desafio associado:</td>
        <td width="98%">
	<textarea rows="2" name="mobdsc" cols="50"><?=$mobdsc?></textarea></td>
	<td></td>
      </tr>
      <tr>
        <td width="1%"></td>
        <td width="25%">Programa estruturante?</td>
        <td width="98%">
	<?
	   if ($prgsnestruturante) {
	      print '<input type="radio" value="1" name="prgsnestruturante" checked>Sim &nbsp;&nbsp; <input type="radio" name="prgsnestruturante" value="0" checked>Não';
	   }
	   else
	   {
	      print '<input type="radio" value="0" name="prgsnestruturante"> Sim &nbsp;&nbsp; <input type="radio" name="prgsnestruturante" value="1"  checked>Não';
	   }
	?>
	<td></td>
      </tr>
      <tr>
        <td width="1%"></td>
        <td width="25%">Tipo:</td>
        <td width="98%">
	<?
            $sql = "select tprcod as CODIGO,tprnome as DESCRICAO from tipoprograma order by tprnome ";
            $db->monta_radio("tprcod",$sql,0)
	?>
	<td></td>
      </tr>
      <tr>
        <td width="1%"></td>
        <td width="25%">Horizonte temporal:</td>
        <td width="98%">
	<?
	   if ($prgsntemporario) {
	      print '<input type="radio" value="0" name="prgsntemporario"> Contínuo &nbsp;&nbsp; <input type="radio" name="prgsntemporario" value="1" checked>Temporário';
	   }
	   else
	   {
	      print '<input type="radio" value="1" name="prgsntemporario" checked> Contínuo &nbsp;&nbsp; <input type="radio" name="prgsntemporario" value="0" >Temporário';
	   }
	?>
	<td></td>
      </tr>
      <tr>
        <td></td>
        <td width="25%">Data Início/Término</td>
        <td width="98%"><input type="text" name="prgmesinicio" size="2" value='<?=$prgmesinicio?>'>
	<input type="text" name="prganoinicio" size="4" value='<?=$prganoinicio?>'>
	<? if ($prgsntemporario) {
	?>
	  <br>(Se temporário) <input type="text" name="prgmestermino" size="2" value='<?=$prgmestermino?>'><input type="text" name="prganotermino" size="4" value='<?=$prganotermino?>'>
	  <?}?>

	  </td><br>
	<td></td>
      </tr>

       <tr>
        <td></td>
        <td width="25%">Valor Global Estimado <br>do Programa (R$) (se temporário)</td>
        <td width="98%">
	<? if ($prgsntemporario) {
	?>
            <input type="text" name="prgvlrprograma" size="10" value='<?=$prgvlrprograma?>'>
	  <?}?>

	  </td><br>
	<td></td>
      </tr>
      <tr>
        <td width="1%"></td>
        <td width="25%">Estratégia de implementação:</td>
        <td width="98%">
	<textarea rows="2" name="prgdscestrategia" cols="50"><?=$prgdscestrategia?></textarea></td>
	<td></td>
      </tr>
    </table>
    <br><br>
<table width='65%' align='center' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
    <tr>
      <td><strong>Código</strong></td>
      <td><strong>Descrição</strong></td>
      <td><strong>Ver ações</strong></td>
    </tr>
<?

$sql= "select prgcod as codigo, prgdsc as descricao from programa where prgano = '".$_SESSION['exercicio']."'  order by prgcod";


$RS2 = $db->record_set($sql);

$nlinhas = $db->conta_linhas($RS2);

for ($i=0; $i<$nlinhas;$i++){
  $res = $db->carrega_registro($RS2,$i);
// a linha abaixo transforma em variáveis todos os campos do array
  if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	
   print '<tr><td><a href=simec?modulo='.$modulo.'&prgcod='.$codigo.'>'.$codigo.'</a></td><td>'.$descricao.'</td>';
   print '<td><a href=simec?modulo=principal/acao/cadacao&prgcod='.$codigo.'>Ver ações</a></td></tr>';
}

?>
    </table>
    </center>
  
<? // inclusão do menu de setas de navegação
include "includes/setas_navegacao.inc";?> 
</form>
<script>
    function VerRegistro(cod) {
	//alert (cod+1);
        document.formulario.navega.value = cod;
	document.formulario.submit();
    }
    function ProcuraPrograma(form) {
	document.formulario.procura.value = document.formulario.prgcod.value;
	document.formulario.submit();
    }
</script>
</body>
</html>