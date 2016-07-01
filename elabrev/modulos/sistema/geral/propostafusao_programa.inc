<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:propostarevisao.inc
   Finalidade: eramitir a construção de propostas de revisão
   */
$modulo=$_REQUEST['modulo'] ;//

$titulo_modulo='Proposta de Fusão de Programa';
monta_titulo($titulo_modulo,'');

$sql= "select a.* from elabrev.ppaprograma_proposta a where a.prgstatus in ('N','A') and a.prgid = ".$_REQUEST['prgid'];


if (! $_REQUEST['prgid'] or $_REQUEST['prgid']=='') {
	?>
	<script>alert('Ocorreu um problema ao abrir a página. Tente novamente!');
	history.back();</script>
	<?
	$db->close();
	exit();
}

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
if ($nlinhas < 0 ){
	?>
	<script>alert('Programa Inexistente!');
	history.back();</script>
	<?
	$db->close();
	exit();}
else {
  $res =  @$db->carrega_registro($RS,0);
}
// Transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

if ($_REQUEST['decisao'])
{
	// decidiu
	$sql = "update elabrev.proposta_fusao_programa set pfpdata=now() ,pfpstatus='D' where pfpid=".$_REQUEST['decisao'];  	 
  	$db->executar($sql);
	$db->commit();
	?>
      <script>
         alert('Gravação com sucesso!')
        // window.close();
      </script>
    <?
 //exit();
}

if ($_REQUEST['cdecisao'])
{
	// cancelou a decisão
	$sql = "update elabrev.proposta_fusao_programa set pfpdata=now(),pfpstatus='A' where pfpid=".$_REQUEST['cdecisao'];  	 
  	$db->executar($sql);
	 $db->commit();
	   ?>
      <script>
         alert('Gravação com sucesso!')
         //window.close();
      </script>
  <?
 // exit();
}

if ($_REQUEST['grava'] and $_REQUEST['pfpjustificativa'])    
{
  if ($_REQUEST['inserir']==1)
  {  	  	  	
  	$prgid_externa = $_REQUEST['prgid_combo'];
  	$sql = "insert into elabrev.proposta_fusao_programa (prgid,usucpf,prgid_externa,pfpjustificativa,prsano)
       values (".
  	$_REQUEST['prgid'].",'".
  	$_SESSION['usucpf']."',".
  	$prgid_externa.",'".
  	$_REQUEST['pfpjustificativa']."','".$_SESSION['exercicio']."')";
  	$db->executar($sql); 

  	// uma vez que ocorre a inclusão, pego o valor do pfpid  	 	
  	$sql = "select pfpid from elabrev.proposta_fusao_programa where prgid=".$_REQUEST['prgid']." and usucpf='".$_SESSION['usucpf']."' and prgid_externa=$prgid_externa ";
	$RS = $db->record_set($sql);
	$res =  @$db->carrega_registro($RS,0);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

    // agora acho os dados identificadores da outra ação
    $sql = "select prgcod as prgcode, prgdsc as prgdsce from elabrev.ppaprograma_proposta where prgid=".$prgid_externa;

	$RS = $db->record_set($sql);
	$res =  @$db->carrega_registro($RS,0);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;	
	
	
  	// inserir uma proposta de exclusão da ação que fundiu
  	
	$sql= "insert into elabrev.proposta_exclusao_programa (usucpf,pepjustificativa,prgid,peporigem,prsano) values ('".
  	$_SESSION['usucpf']."',".
  	"'Fusão com a ação $prgcod',$prgid_externa,'F','".$_SESSION['exercicio']."')";
    $db->executar($sql); 	
	$db->commit();
	$pfpid=0;
	?>
      <script>
         alert('Gravação com sucesso.')
      </script>
  <?
  //exit();
  }
  if ($_REQUEST['inserir']==2)
  {
     $sql = "update elabrev.proposta_fusao_programa set pfpdata=now(), pfpjustificativa='".$_REQUEST['pfpjustificativa']."' where pfpid=".$_REQUEST['pfpid'];
  	 $db->executar($sql);
	 $db->commit();
	   ?>
      <script>
         alert('Gravação com sucesso.')
      </script>
  <?
    }
}


if ($_REQUEST['edita'] and $_REQUEST['inserir']==3)    
{
	$sql = "select * from proposta_fusao_programa where pfpid=".$_REQUEST['edita'];
	$RS = $db->record_set($sql);
	$res =  @$db->carrega_registro($RS,0);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
  	
    $sql="select distinct pfpid from elabrev.proposta_fusao_programa p where p.pfpstatus in ('A','D') and p.prgid=".$_SESSION['prgid']." and usucpf='".$_SESSION['usucpf']."'"; 	 
	$RS = $db->record_set($sql);
	$nlinhas=$db->conta_linhas($RS);
	
	$sql = "delete from elabrev.proposta_fusao_programa where pfpid=".$_REQUEST['edita'];
  	$db->executar($sql);

	$sql= "delete from elabrev.proposta_exclusao_prgo where usucpf='".$_SESSION['usucpf']."' and prgid=$prgid_externa and peporigem='F'";
  	$db->executar($sql);
	   $db->commit();
	     	   $pfpid=0;
  	   $pfpjustificativa='';
	   ?>
      <script>
         alert('Exclusão realizada com sucesso!')
      </script>
  <?

}
$habil='S';
if ($_REQUEST['edita'] and $_REQUEST['inserir']==2)    
{
	$sql = "select * from proposta_fusao_programa where pfpid=".$_REQUEST['edita'];
	$RS = $db->record_set($sql);
	$res =  @$db->carrega_registro($RS,0);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	$prgid_combo=$prgid_externa;
	$habil='N';
}
?>

<html>
<head>
<title>Proposta de Fusão</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="decisao" value="0">
<input type=hidden name="cdecisao" value="0">
<input type=hidden name="grava" value="0">
<input type=hidden name="edita" value="0">
<? if ($pfpid) {?>
<input type=hidden name="inserir" value="2">
<input type=hidden name="pfpid" value="<?=$pfpid?>">
<?} else {?>
<input type=hidden name="inserir" value="1">
<?}?>
<center>
    <table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
      <tr>
        <td align='right' class="subtitulodireita">Programa:</td>
        <td><?=$prgcod.'-'.$prgdsc?></td>
      </tr>  
 	  <tr>
        <td align="right" class="subtitulodireita">Escolha o programa:</td>
        <? 
        if ($habil <> 'N')
           $sql = "select distinct a.prgid as codigo, a.prgcod||'-'||a.prgdsc as descricao from elabrev.ppaprograma_proposta a where a.orgcod='".$_SESSION['ittorgao']."' and a.prgid !=".$_SESSION['prgid']." and a.prgstatus ='A' and a.prgid not in (select pf.prgid_externa from elabrev.proposta_fusao_programa pf where pf.usucpf='".$_SESSION['usucpf']."' and pf.prsano='".$_SESSION['exercicio']."') order by descricao";
        else
        { 
           $sql = "select distinct a.prgid as codigo, a.prgcod||'-'||a.prgdsc as descricao from elabrev.ppaprograma_proposta a where a.orgcod='".$_SESSION['ittorgao']."' and a.prgid !=".$_SESSION['prgid']." and a.prgstatus='A'  order by descricao";
           
        }

        ?><td colspan=2><?=$db->monta_combo("prgid_combo",$sql,$habil,"Escolha o programa que será fundido.",'','',$explicprgo,400);?>   
      
        </td>
     </tr>
	  <tr>
        <td align="right" class="subtitulodireita">Justificativa:</td> 
       <td colspan=2><?=campo_textarea('pfpjustificativa','N','S','','80',5,'');?></td>
     </tr>
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita">
	 <? if (! $pfpid) {?>
	 <input type='button' class="botao" value='Inserir!' onclick="grava_proposta()">
	 <?} else {?>
	 <input type='button' class="botao" value='Alterar!' onclick="grava_proposta()">	 
	 <?}?>
	 &nbsp;&nbsp;&nbsp;<input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>
	 </tr>
  </table>
</form> 

<?
// lista as propostas já feitas
$cabecalho = array('Status','Autor','Proposta de Fusão');
if ($db->verifica_momento() and $db->verifica_resp_elabrev())
	$sql="select distinct case when p.pfpstatus='A' then '<img border=\"0\" src=\"../imagens/principal.gif\" title=\" Editar proposta \" onclick=\"edita_proposta(' || p.pfpid|| ')\">
	<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir proposta \" onclick=\"exclui_proposta(' || p.pfpid|| ')\">	
	<img border=\"0\" src=\"../imagens/IconeAjuda.gif\" title=\" Ver justificativa \" onclick=\"ver_justificativa(' || p.pfpid|| ')\">' else 
	'<img border=\"0\" src=\"../imagens/IconeAjuda.gif\" title=\" Ver justificativa \" onclick=\"ver_justificativa(' || p.pfpid|| ')\">' end as prgo ,'<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || u.usucpf || '\'' || ')\">&nbsp;'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum as autor, a.prgdsc||'<br> Digitado em '||to_char(p.pfpdata,'YYYY/mm/dd') as dado 
from elabrev.proposta_fusao_programa p inner join seguranca.usuario u on p.usucpf=u.usucpf inner join elabrev.ppaprograma_proposta a on a.prgid=p.prgid_externa where p.pfpstatus in ('A','D') and p.prgid=".$_SESSION['prgid']." order by dado desc, prgo, autor";
else 
	$sql="select distinct  '<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || u.usucpf || '\'' || ')\"><img border=\"0\" src=\"../imagens/ajuda.gif\" title=\" Ver justificativa \" onclick=\"ver_justificativa(' || p.pfpid|| ')\">' as prgo,case when p.pfpstatus='A' then 'Prop.' when p.pfpstatus='D' then 'Dec.' end as status,u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum as autor,to_char(p.pfpdata,'YYYY/mm/dd') as data, '<br> Digitado em '||to_char(p.pfpdata,'YYYY/mm/dd') as dado from elabrev.proposta_fusao_programa p inner join seguranca.usuario u on p.usucpf=u.usucpf inner join elabrev.ppaprograma_proposta a on a.prgid=p.prgid_externa where p.pfpstatus in ('A','D') and  p.prgid=".$_SESSION['prgid']."  and p.prsano='".$_SESSION['exercicio']."' order by dado desc, prgo, autor";

    $db->monta_lista($sql,$cabecalho,100,20,'','','');

?>

<script>
  function fechar_janela()
  {
    window.close();

  }
    function grava_proposta()
  {
  	try
  	{
  	    if (!validaBranco(document.formulario.prgid_combo, 'Ação')) return;
  	}
  	catch(e)
		{
			
		}
  	if (!validaBranco(document.formulario.pfpjustificativa, 'Justificativa')) return;  	
	document.formulario.grava.value = 1;
	document.formulario.submit();
  }
    function edita_proposta(cod)
  {
  	document.formulario.inserir.value = 2;  
	document.formulario.edita.value = cod;
	document.formulario.submit();
  }  
    function exclui_proposta(cod)
  {
	document.formulario.inserir.value = 3;  	
	document.formulario.edita.value = cod;
	document.formulario.submit();
  }  
   function envia_email(cpf)
  {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&prgo=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
  }

  function ver_justificativa(cod)
  {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/ver_justificativa&prgo=A&tp=2&cod="+cod;          
          window.open(e, "Justificativa","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
  }    

  function tomar_decisao(cod)
  {
  	if( window.confirm( "Confirma a decisão?") )
    {
	    document.formulario.decisao.value = cod;
	    document.formulario.submit();
    } else return;
  }  
      
  function cancelar_decisao(cod)
  {
    if( window.confirm( "Confirma o CANCELAMENTO da decisão?") )
    {
	    document.formulario.cdecisao.value = cod;
	    document.formulario.submit();
    } else return;
  }
        
</script>
</body>
</html>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
