<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:monitoraprg.inc
   finalidade: permitir o monitoramento de programa
   */
$modulo=$_REQUEST['modulo'] ;//


if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];

$sql="select prgid from programa where prgdsc like '%(RAP%' and prgid=".$_SESSION['prgid'];
$registro=$db->recuperar($sql);
if (is_array($registro)) {
?>
	<script>alert('Este Programa é de Restos a Pagar e não é monitorado!');
	history.back();</script>
	<?
	$db->close();
	exit();
}
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];
include APPRAIZ."includes/cabecalho.inc";

// testar se houve solicitação de gravar
if ($_REQUEST['tipograva']== 'A')
{
     // é uma avaliação
     // verifico se o cod é zero, se for, será um insert, senão será update
     $cor =  $_REQUEST['corcodav'.$_REQUEST['avptexto']];
     $sit =  $_REQUEST['tpscodav'.$_REQUEST['avptexto']];
     if ($_REQUEST['doccod']== '0')
     {
       $texto = stripslashes($_REQUEST['avaliacao_'.$_REQUEST['refren'].'_0_'.$_REQUEST['avptexto']]);
       $sql= "insert into avaliacaoparecer (tpaid,avporigem,refcod,usucpf,corcod,tpscod,prgid,avptexto,avpstatus,avpdata) values (1,1,".$_REQUEST['refren'].",'".$_SESSION['usucpf']."',".$cor.",".$sit.",".$_SESSION['prgid'].",'".$texto."',"."'A','".date('Y/m/d')."')";
     }
     else
     {
       $texto = stripslashes($_REQUEST['avaliacao_'.$_REQUEST['refren'].'_'.$_REQUEST['doccod'].'_'.$_REQUEST['avptexto']]);
       $sql= "update avaliacaoparecer set refcod=".$_REQUEST['refren'].",usucpf='".$_SESSION['usucpf']."',corcod=".$cor.",tpscod=".$sit.",prgid=".$_SESSION['prgid'].",avptexto='".$texto."',avpdata='".date('Y/m/d')."' where avpid=".$_REQUEST['doccod'];
     }
}
if ($_REQUEST['tipograva']== 'P')
   {
     // é um parecer
     // verifico se o cod é zero, se for, será um insert, senão será update
       $cor =  $_REQUEST['corcodpar'.$_REQUEST['avptexto']];
       $sit =  $_REQUEST['tpscodpar'.$_REQUEST['avptexto']];
     if ($_REQUEST['doccod']== '0')
     {
       $texto = stripslashes($_REQUEST['parecer_'.$_REQUEST['refren'].'_0_'.$_REQUEST['avptexto']]);
       $sql= "insert into avaliacaoparecer (tpaid,avporigem,refcod,usucpf,corcod,tpscod,prgid,avptexto,avpstatus,avpdata) values (2,2,".$_REQUEST['refren'].",'".$_SESSION['usucpf']."',".$cor.",".$sit.",".$_SESSION['prgid'].",'".$texto."',"."'A','".date('Y/m/d')."')";
     }
     else
     {
       $texto = stripslashes($_REQUEST['parecer_'.$_REQUEST['refren'].'_'.$_REQUEST['doccod'].'_'.$_REQUEST['avptexto']]);
       $sql= "update avaliacaoparecer set refcod=".$_REQUEST['refren'].",usucpf='".$_SESSION['usucpf']."',corcod=".$cor.",tpscod=".$sit.",prgid=".$_SESSION['prgid'].",avptexto='".$texto."',avpdata='".date('Y/m/d')."' where avpid=".$_REQUEST['doccod'];
     }
}
if ($_REQUEST['tipograva']== 'LA')
{
     // é a liberação de uma avaliação
     $cor =  $_REQUEST['corcodav'.$_REQUEST['avptexto']];
     $sit =  $_REQUEST['tpscodav'.$_REQUEST['avptexto']];
     if ($_REQUEST['doccod']== '0')
       $texto = stripslashes($_REQUEST['avaliacao_'.$_REQUEST['refren'].'_0_'.$_REQUEST['avptexto']]);
     else
       $texto = stripslashes($_REQUEST['avaliacao_'.$_REQUEST['refren'].'_'.$_REQUEST['doccod'].'_'.$_REQUEST['avptexto']]);
       $sql= "update avaliacaoparecer set avpliberada='t',refcod=".$_REQUEST['refren'].",usucpf='".$_SESSION['usucpf']."',corcod=".$cor.",tpscod=".$sit.",prgid=".$_SESSION['prgid'].",avptexto='".$texto."',avpdata='".date('Y/m/d')."' where avpid=".$_REQUEST['doccod'];
     
     
}
if ($_REQUEST['tipograva']== 'BA' or $_REQUEST['tipograva']== 'BP')
{
     // é bloqueio de uma avaliação ou parecer liberado
     $texto = 'texto';
     $sit='1';
     $cor='3';
     $sql= "update avaliacaoparecer set avpliberada='f' where avpid=".$_REQUEST['doccod'];
}
if ($_REQUEST['tipograva']== 'LP')
{
     // é a liberação de um parecer
     $cor =  $_REQUEST['corcodpar'.$_REQUEST['avptexto']];
     $sit =  $_REQUEST['tpscodpar'.$_REQUEST['avptexto']];
     if ($_REQUEST['doccod']== '0')
       $texto = stripslashes($_REQUEST['parecer_'.$_REQUEST['refren'].'_0_'.$_REQUEST['avptexto']]);
     else
       $texto = stripslashes($_REQUEST['parecer_'.$_REQUEST['refren'].'_'.$_REQUEST['doccod'].'_'.$_REQUEST['avptexto']]);
       $sql= "update avaliacaoparecer set avpliberada='t', refcod=".$_REQUEST['refren'].",usucpf='".$_SESSION['usucpf']."',corcod=".$cor.",tpscod=".$sit.",prgid=".$_SESSION['prgid'].",avptexto='".$texto."',avpdata='".date('Y/m/d')."' where avpid=".$_REQUEST['doccod'];
     
     
}
if ($_REQUEST['tipograva']== 'A' or $_REQUEST['tipograva']== 'P' or $_REQUEST['tipograva']== 'LA' or $_REQUEST['tipograva']== 'LP' or $_REQUEST['tipograva']== 'BA' or $_REQUEST['tipograva']== 'BP')
{
      if (ereg_replace("<[^>]*>","",$texto) == '') { 
	   ?>
	      <script>
	         alert ('A Avaliação ou Parecer não pode estar em branco.');
	         history.back();
	      </script>
	   <?
	     exit();
	   } ?>

	   <?
       if ($sit == '') {
	   ?>
	      <script>
	         alert ('Escolha uma situação. Campo obrigatório');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
       if ($cor == '') {
	   ?>
	      <script>
	         alert ('Escolha uma das cores. Campo Obrigatório');
	         history.back();
	      </script>
	   <?
	     exit();
	   }

     $db->executar($sql);
     $db->commit();
     ?>
	 <script language="JavaScript">alert('Operação Realizada com Sucesso!');</script>
	 <?
}
?>
<title>MONITORAR PROGRAMAS</title>
<br>
<?
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Avaliação e Parecer de Programa';
monta_titulo($titulo_modulo,'');
$sql= "select * from programa where prgstatus='A' and prgano = '".$_SESSION['exercicio']."'  order by prgcod";
if ($_REQUEST['prgid']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgid=".$_REQUEST['prgid']."  order by prgcod";
if ($_POST['procura']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$_POST['procura']."'  order by prgcod";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
// as linhas abaixo não permitem navegar para além dos limites
//controla_navegacao($nlinhas);
if ($nlinhas < 0 ){
	?>
	<script>alert('Programa Inexistente!');
	history.back();</script>
	<?
	$db->close();
	exit();}
// fim do controle de navegação
$res =  $db->carrega_registro($RS,0);
// a linha abaixo transforma em variÃ¡veis todos os campos do array
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v; 
	$_SESSION['prgid']=$prgid;
	    // encontrar o orgao
    $sql= "select orgdsc from orgao where orgcod='".$orgcod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar a unidade
    $sql= "select unidsc from unidade where unicod='".$unicod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar o tipo de programa
if (! $_REQUEST['refcod'])
{
  $sql = "select refcod from referencia where refsnmonitoramento='t' and refdata_inicio < now()::date and refano_ref='".$_SESSION['exercicio']."' order by refano_ref desc,refmes_ref desc";
$RS = $db->record_set($sql);
$res =  $db->carrega_registro($RS,0);
$_REQUEST['refcod']= $res[0];

}
?>
<form method="POST"  name="formulario" onsubmit="ProcuraPrograma();">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="refren" value=0>
<input type=hidden name="doccod" >
<input type=hidden name="tipograva">
<input type=hidden name="procura">
<input type=hidden name="avptexto" value=0>
<input type=hidden name="partexto" value=0>
<center>
<script language="javascript" type="text/javascript" src="includes/tiny_mce_gzip.php"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	mode : "textareas",
	theme : "advanced",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen,zoom",
	theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, print,preview,fullscreen,separator,zoom",
	theme_advanced_buttons2 : "",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
	language : "pt_br",
	entity_encoding : "raw",
	width : "100%"
	});
</script>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
      <tr>
        <td align='right' class="subtitulodireita" width="20%">Código:</td>
        <td><?=campo_texto('prgcod','N','S','',10,100,'','');?>
	&nbsp;&nbsp;<img border="0" src="imagens/busca.gif"  alt='procura programa' title='procura programa' onclick="ProcuraPrograma()">
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Orgão Responsável:</td>
        <td><?=$orgdsc?></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Unidade Responsável:</td>
         <td><?=$unidsc?></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Denominação:</td>
        <td><?=$prgdsc?></td>
      </tr>
<?
if ($_REQUEST['refcod']) $refcod=$_REQUEST['refcod'];
   $sql = "select pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra, usu.usunome || '('|| org.orgdsc ||')' as usuario from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.prgid = ".$_SESSION['prgid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where  pfl.pflstatus='A' and pfl.pflresponsabilidade in ('P','X','D') order by pfl.pfldsc";
    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
            if (! $mostra ){
	        $linha = "<tr><td align='right' class='SubTituloDireita'>".$descricao.":</td><td>".$usuario."</td></tr>";
            print $linha;
            }
        }
     }
?>
<tr><td align='right' class="SubTituloDireita">Período de Referência :</td>
<td>
<?
$sql2 = "select refcod as CODIGO,refdsc as DESCRICAO from referencia where refdata_limite_parecer_prg is not null and refdata_limite_avaliacao_prg is not null and refsnmonitoramento='t'   and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref desc";
$db->monta_combo("refcod",$sql2,'S',"Escolha o período de referência",'Escolhe_referencia()','Todas os períodos de Referências');
?>
</td></tr>
  </table>
  <br>
  <?
    $sql = "select ref.*,av.tpscod as avtps,av.corcod as avcor, av.avpid as avaid, av.tpaid as tpav, av.avptexto as avaliacao, ";
	$sql = $sql." av.avpliberada as avlib,av.usucpf as avusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as avdata, ca.corimgav, ca.corsignificado as avcordsc, ca.cordsc as avcornome,  tsa.tpsdsc as avtpsdsc, pa.tpscod as partps, tsp.tpsdsc as partpsdsc ,pa.corcod as parcor, ";
	$sql = $sql." pa.avpid as parid, pa.avpliberada as parlib, pa.tpaid as tppar, pa.avptexto as parecer,pa.usucpf as parusu, to_char(av.avpdata,'DD/MM/YYYY HH:MM') as pardata, ";
	$sql = $sql." cp.corimgpar, cp.corsignificado as pacordsc, cp.cordsc as parcornome, tsa.tpsdsc as patpsdsc ";
	$sql = $sql." from referencia ref ";
	$sql = $sql." left join avaliacaoparecer av on av.refcod=ref.refcod and av.tpaid=1 and av.prgid = ".$_SESSION['prgid'];
	$sql = $sql." left join cor ca on av.corcod = ca.corcod ";
	$sql = $sql." left join tiposituacao tsa on tsa.tpscod = av.tpscod ";
	$sql = $sql." left join avaliacaoparecer pa on pa.refcod=ref.refcod and pa.tpaid=2 and pa.prgid = ".$_SESSION['prgid'];
	$sql = $sql." left join cor cp on pa.corcod = cp.corcod ";
	$sql = $sql." left join tiposituacao tsp on tsp.tpscod = pa.tpscod";
	$sql = $sql." where ref.refdata_limite_parecer_prg is not null and ref.refdata_limite_avaliacao_prg is not null ";
	if ($_REQUEST['refcod'] and $_REQUEST['refcod'] <> 'x')
        $sql = $sql." and ref.refsnmonitoramento='t' and ref.refano_ref='".$_SESSION['exercicio']."' and ref.refcod=".$_REQUEST['refcod']." order by refano_ref desc,refmes_ref desc ";
else if ($_REQUEST['refcod'])
		$sql = $sql." and ref.refsnmonitoramento='t' and ref.refano_ref='".$_SESSION['exercicio']."' order by refano_ref desc,refmes_ref desc ";
		
  $RS = $db->record_set($sql);
  $nlinhas = $db->conta_linhas($RS);
  // com o prgid e o usucpf da sessão posso saber se o usuário terá ou não acesso à edição dos registros.
  // verifica se é gerente
  if ($db->testa_gerente($_SESSION['prgid'],'P')) $autorizado_par = true;
  // verifica se é gerente-executivo
  if ($db->testa_gerente_exe($_SESSION['prgid'],'P')) $autorizado_av = true;
  // verifica se é digitador
  if ($db->testa_digitador($_SESSION['prgid'],'P')) $autorizado_dig = true;
  // caso a autorização não tenha sido feita, testo se é super usuário
  if ($db->testa_superuser() ) {
     $autorizado_av = true;
     $autorizado_par = true;
  }
//print $sql;

?>
<table border="0" cellspacing="0" cellpadding="0" align="center" bgcolor="#F5F5F5" class="tabela">
  <tbody>
   <?
   if ($_REQUEST['refcod'])
   {
  for ($i=0;$i<=$nlinhas;$i++)
  {
    $habil= 'N';
   $res =  $db->carrega_registro($RS,$i);
   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
   if ($avlib<>'t') $avcornome="";
   if ($parlib<>'t') $parcornome="";
?>
   <tr align="center" bgcolor="#DCDCDC"><td colspan="2" style="BORDER-TOP: #000000 2px solid; BORDER-BOTTOM: #cccccc 1px solid; color:#006699; font-size: 10pt; padding: 3px;" align="left"><img align="top" src="imagens/Sin_<?=trim($avcornome).'_'.trim($parcornome)?>.gif" border="0"> <strong>Período de Referência <?=$refdsc?></strong> </td></tr>
   <tr><td width="50%" bgcolor="#e9e9e9" align="center" style="BORDER-RIGHT: #cccccc 1px solid;color:#990000;" valign="top">Avaliação</td><td width="50%" align="center" bgcolor="#e9e9e9" style="color:#990000;" valign="top">Parecer</td></tr>
   <tr>
   <td align="center" style="BORDER-RIGHT: #cccccc 1px solid;" valign="top">
<?   
   // incia a avaliação *********************************************
    include "modulos/principal/programa/avalia_prg.inc";
   // *******************************************************************//
   //   divide avaliação de parecer
?>
</td><td align="center" style="BORDER-RIGHT: #cccccc 1px solid;" valign="top">
<?
   // aqui inicia parecer
   //********************************************************************//
   include "modulos/principal/programa/parecer_prg.inc";
	 // termina parecer ***********************
   print '</tr>';
  }}
  ?>
</tbody>
  </table>
  </center>
</div>
</form>
<script>
    function verregistro(cod) {
        document.formulario.navega.value = cod;
	document.formulario.submit();
    }
    function ProcuraPrograma() {
	document.formulario.procura.value = document.formulario.prgcod.value;
	document.formulario.submit();
    }
    function grava_av(refcod,cod,i)
    {
      document.formulario.refren.value=refcod;
      document.formulario.tipograva.value='A';
      document.formulario.doccod.value=cod;
      document.formulario.avptexto.value = i;
      document.formulario.submit();
    }
    function grava_par(refcod,cod,i)
    {
      document.formulario.refren.value=refcod;
      document.formulario.tipograva.value='P';
      document.formulario.doccod.value=cod;
      document.formulario.avptexto.value = i;
      document.formulario.submit();
    }
    function libera_av(refcod,cod,i)
    {
      document.formulario.refren.value=refcod;
      document.formulario.tipograva.value='LA';
      document.formulario.doccod.value=cod;
      document.formulario.avptexto.value = i;
      document.formulario.submit();
    }
    function libera_par(refcod,cod,i)
    {
      document.formulario.refren.value=refcod;
      document.formulario.tipograva.value='LP';
      document.formulario.doccod.value=cod;
      document.formulario.avptexto.value = i;
      document.formulario.submit();
    }
    function bloquea_av(refcod,cod,i)
    {
      document.formulario.refren.value=refcod;
      document.formulario.tipograva.value='BA';
      document.formulario.doccod.value=cod;
      document.formulario.avptexto.value = i;
      document.formulario.submit();
    }
    function bloquea_par(refcod,cod,i)
    {
      document.formulario.refren.value=refcod;
      document.formulario.tipograva.value='BP';
      document.formulario.doccod.value=cod;
      document.formulario.avptexto.value = i;
      document.formulario.submit();
    }
	function visualiza(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/mostraavpar&acao=A&cod="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=480'");
	}
    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
    function Escolhe_referencia()
	{
      	document.formulario.submit();
 }
</script>
