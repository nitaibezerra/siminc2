<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:inclui_unid.inc
   Finalidade: incluir unidades e localizadores em uma nova ação
   */
$modulo=$_REQUEST['modulo'] ;//


$titulo_modulo='Incluir Unidades Orçamentárias';
monta_titulo($titulo_modulo,'');


if (! $_REQUEST['acaid'] or $_REQUEST['acaid']=='') {
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
	<script>alert('Ação Inexistente!');
	history.back();</script>
	<?
	$db->close();
	exit();}
else {
  $res =  @$db->carrega_registro($RS,0);
}
// Transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;


if ($_REQUEST['grava'])    
{
  if ($_REQUEST['inserir']==1)
  {  	  	  	
  	$sql = "insert into elabrev.unidade_acao (acaid,usucpf,unicod,uacstatus)
       values (".
  	$_REQUEST['acaid'].",'".
  	$_SESSION['usucpf']."','".
  	$_REQUEST['unicod']."','N')";

  	$db->executar($sql); 

	?>
      <script>
         alert('Gravação com sucesso.')
      </script>
  <?
  //exit();
  }
  if ($_REQUEST['inserir']==3)
  {  	  	  	
  	$sql = "delete from elabrev.unidade_acao where uacstatus='N' and acaid=".$_REQUEST['exclui']." and unicod='".$_REQUEST['grava']."'";
  	$db->executar($sql); 

	?>
      <script>
         alert('Exclusão realizada com sucesso.')
      </script>
  <?
  //exit();
  }  

}



$habil='S';
$sql= "select a.* from elabrev.ppaacao_proposta a where a.acastatus='N' and a.acaid = ".$_REQUEST['acaid'];

$RS = $db->record_set($sql);
$res =  @$db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

?>

<html>
<head>
<title>Inclusão de Unidades e Localizadores</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="grava" value="0">
<input type=hidden name="inserir" value="0">
<input type=hidden name="exclui" value="0">
<input type=hidden name="acaid" value="<?=$_REQUEST['acaid']?>">


<center>
    <table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
      <tr>
        <td colspan="2">A inclusão da Unidade exige seguir o passo-a-passo de filtragem:</td>
      </tr>     
      <tr>
        <td align='right' class="subtitulodireita">Ação:</td>
        <td><?=$prgcod.'.'.$acacod.'-'.$acadsc?></td>
      </tr>  
 	  <tr>
        <td align="right" class="subtitulodireita">Escolha o órgão:</td>
        <td>	<?
   $orgcod =$_REQUEST["orgcod"];
    $sql = "select orgcod as CODIGO,orgdsc as DESCRICAO from orgao order by orgdsc ";
	  $db->monta_combo("orgcod",$sql,$habil,"Todos os Órgãos",'atualizaComboUnidade','','',400);
	?>     
        </td>
     </tr>
     
	<?
	  if ($_REQUEST["orgcod"]) {
   $unicod =$_REQUEST['unicod'];
        ?>
      <tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Unidade Orçamentária (UO):</td>
         <td >
         <?
		$sql="select u.unicod as CODIGO,u.unicod||' - '||u.unidsc as DESCRICAO 
from unidade u where u.unicod not in (select ua.unicod from  elabrev.unidade_acao ua where ua.acaid=".$_REQUEST['acaid'].") and u.unistatus='A' and u.unitpocod='U' and u.orgcod ='".$_REQUEST["orgcod"]."' and u.unitpocod='U' order by descricao";
$db->monta_combo("unicod",$sql,'S',"Selecione a Unidade ",'atualizaComboUnidade','','',400);
?>
	</td>
      </tr>
<?}?>     
     
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita">

	 <input type='button' class="botao" value='Inserir!' onclick="grava_proposta()">
	 &nbsp;&nbsp;&nbsp;<input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>
	 </tr>
  </table>
</form> 

<?
// lista as propostas já feitas
$cabecalho = array('Status','Unidade','Proponente');
if ($db->testa_superuser() or $db->testa_gerente_exe($_REQUEST['acacod'],'A'))
	$sql = "select distinct '<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir proposta\" onclick=\"exclui_proposta(' || ua.unicod||','||ua.acaid||')\">',u.unicod || '-'||u.unidsc as dado,'Proposto por ' || usu.usucpf ||'-'||usu.usunome as proponente from elabrev.unidade_acao ua inner join unidade u on u.unicod=ua.unicod inner join seguranca.usuario usu on usu.usucpf=ua.usucpf where ua.acaid=".$_REQUEST['acaid']." and usu.usucpf='".$_SESSION['usucpf']."' order by proponente";
else 
	$sql = "select distinct '',u.unicod || '-'||u.unidsc as dado,'Proposto por ' || usu.usucpf ||'-'||usu.usunome as proponente from elabrev.unidade_acao ua inner join unidade u on u.unicod=ua.unicod inner join seguranca.usuario usu on usu.usucpf=ua.usucpf where ua.acaid=".$_REQUEST['acaid']."  order by proponente";

	

    $db->monta_lista($sql,$cabecalho,100,20,'','','');

?>

<script>
  function fechar_janela()
  {
    window.close();
    opener.location.reload();

  }
    function grava_proposta()
  {
  	if (!validaBranco(document.formulario.unicod, 'Unidade Orçamentária')) return;	
	document.formulario.grava.value = 1;
  	document.formulario.inserir.value = 1; 	
	document.formulario.submit();
  }
  
    function exclui_proposta(cod, cod2)
  {
  	
  	    if( window.confirm( "Confirma a exclusão da Unidade?") )
    {
  	
	document.formulario.inserir.value = 3;  	
	document.formulario.grava.value = cod;
	document.formulario.exclui.value = cod2;	
	document.formulario.submit();
    } else document.formulario.exclui.value = 0;
  	

  }  
   function envia_email(cpf)
  {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
  }
  function atualizaComboUnidade(cod)
  {
	 if (cod > 0) {document.formulario.submit()};
    }    

  
      

        
</script>
</body>
</html>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
