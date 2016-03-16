<? 
 /*
   Sistema Simec
   Setor respons·vel: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Alexandre Soares Diniz
   Programador: Alexandre Soares Diniz
   MÛdulo:celulas.inc
   Finalidade: permitir o cadastro dos celulas
   */

/*
########### A«’ES associaÁ„o de categorias ###########
$modulo=$_REQUEST['modulo'] ;
if ($_REQUEST['act2']=='associar' and $_POST['cdcidpai']<>'') {
  $sql = 'delete from public.combodadoscombinacao where cdcidpai='.$_POST['cdcidpai'];

  $db->executar($sql);
  $cdcidfilho = $_POST['cdcidfilho'];
  //print_r($_POST['cdcidfilho']);
  $nlinhas = count($cdcidfilho)-1;

	for ($j=0; $j<=$nlinhas;$j++)
		{
		  $sql = "insert into public.combodadoscombinacao (cdcidpai, cdcidfilho) 
		  		  VALUES (".$_POST['cdcidpai'].",".$_POST['cdcidfilho'][$j].")
		  		  RETURNING cdcid";
		 // print $_POST['cdcidfilho'][$j].'<br>';
		  //$db->executar($sql);
		  $cdcid = $db->pegaUm($sql);
		 }
  $db -> commit();
  $db->sucesso($modulo);
}
*/

########### A«’ES Cadastro/AtualizaÁ„o de categorias ###########
if (($_REQUEST['act'] == 'inserir') and (! is_array($msgerro))):
	
    $sql= "SELECT
  	  	   cdtid 
  		  FROM
  		   public.combodadostabela 
  		  WHERE
  		   cdtdsc = '".$_REQUEST['dsc']."' 
  		   AND ctbid = ".$_REQUEST['ctbid']."
  		   ;";
   $usu = $db->recuperar($sql);
   unset($sql);

   if (is_array($usu)) {
	   // existe perfil identico, logo, tem que bloquear
	   ?>
	      <script>
	         alert ('A categoria: <?=$_REQUEST['dsc']?> j· est· cadastrado.');
	         history.back();
	      </script>
	   <?
	     exit();
	}


		   
///////// Cadastro de categorias //////////	   
    $sql = "INSERT INTO  public.combodadostabela (ctbid, cdtcod, cdtdsc ) 
    		VALUES (".$_REQUEST['ctbid'].",
    				'".$_REQUEST['cod']."',
    				'".$_REQUEST['dsc']."') 
    		RETURNING cdtid";
   $sisidNew = $db->pegaUm($sql);
   $db->commit();
   unset($sql);
   
    ?>
    	<script>
            alert('OperaÁ„o realizada com sucesso!');
		  	location.href='monitora.php?modulo=sistema/tabapoio/categorias&acao=A';
        </script>
    <?
	exit();
endif;

if ( ($_REQUEST['act']=='alterar') and (! is_array($msgerro)) ):
   // fazer alteraÁ„o de perfil na base de dados.
  	$sql = "UPDATE 
   			 public.combodadostabela 
   			SET 
		   	 cdtcod 		= '".$_REQUEST['cod']."',
		   	 cdtdsc 		= '".$_REQUEST['dsc']."',
		  	 cdtstatus 	= '".$_REQUEST['status']."'
		  	WHERE
		  	 cdtid = ".$_POST['cdtidEdit'];
    $saida = $db->executar($sql);
	$db->commit();
	?>
    	<script>
            alert('OperaÁ„o realizada com sucesso!');
		   	location.href='monitora.php?modulo=sistema/tabapoio/categorias&acao=A&ctbid=<?=$_REQUEST['ctbid']?>';
        </script>
    <?
	exit();
endif;

if ( ($_REQUEST['act']=='excluir') and (! is_array($msgerro)) ):
   // fazer exclus„o de perfil na base de dados.
   	
	$sql1 = "SELECT count(cdcid) as total 
			 FROM public.combodadoscombinacao AS p
			 WHERE cdcidpai = ".$_REQUEST['cdtid']."
			 OR cdcidfilho = ".$_REQUEST['cdtid'];
	 
	$total = $db->PegaUm($sql1);
 	 
	if($total > 0 )
	{
		?>
		<script >
			alert('OperaÁ„o n„o pode ser realizada devido a dados vinculados ao ComboDadosCombinacao');
			location.href = 'monitora.php?modulo=sistema/tabapoio/categorias&acao=A';
		</script>
		<? 
	}else{
     
    $sql  = "DELETE FROM public.combodadostabela WHERE cdtid = ".$_REQUEST['cdtid'];
    $saida = $db->executar( $sql );
    $db->commit();
	?>
    	<script>
            alert('OperaÁ„o realizada com sucesso!');
		   	location.href='monitora.php?modulo=sistema/tabapoio/categorias&acao=A';
        </script>
    <?
	}
	exit();
endif;

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ."includes/cabecalho.inc";
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Cadastro de Categorias';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo ObrigatÛrio.');
?>
<div align="center">
<center>

<?
//atribui valores as variaveis
$modulo=$_REQUEST['modulo'] ;//
//$ctbid = md5_decrypt($_REQUEST['sisidEdit'],'');
$cdtid = $_REQUEST['cdtid'];
$ctbid = $_REQUEST['ctbid'];
$tcbid = $_REQUEST['tcbid'];

if ($cdtid):
	$sql= "SELECT
			 c.cdtid as cdtid,
			 c.ctbid as ctbid,
			 c.cdtcod as cod,
			 c.cdtdsc as dsc,
			 c.cdtstatus AS status,
			 cb.ctbidfilho as ctbidfilho
		   FROM
		     public.combodadostabela c
		     inner join public.COMBOTABELA cb on cb.ctbid = c.ctbid
		   WHERE 
		     c.cdtid = ".$cdtid."
		   ";
	$dados = (array) $db->carregar($sql);
	
	#### Transforma $dados[0] em Vari·veis ####
	extract($dados[0]);
	
else:
	$cdtid 		   = "";
	//$ctbid 	   = "";
	//$ctbidfilho  = "";
	$cod		   = "";
	$dsc   		   = "";
	$status        = "A";
endif;

$act = '';
	
?>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="cdtidEdit" value="<?=$cdtid?>">
<input type='hidden' name="act" value=<?=$act?>>
    <center>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="SubTituloDireita">Tipo:</td>
	     <td>
	     	<?
	     		$sql = "SELECT tcbid as CODIGO, tcbdsc as DESCRICAO  FROM tipocombo;";
	     		$db->monta_combo("tcbid",$sql,'S',"-- Selecione o Tipo --",'filtratipo','');
	     		echo obrigatorio();
	     	?>
	     </td>
      </tr>
      <?if($tcbid){ ?>
      <tr>
        <td align='right' class="SubTituloDireita">Categoria:</td>
	     <td>
	     	<?
	     		$sql = "SELECT ctbid as CODIGO, ctbcampo ||' - '|| ctbdsc as DESCRICAO  FROM COMBOTABELA where tcbid=".$tcbid;
	     		$db->monta_combo("ctbid",$sql,'S',"-- Selecione o Categoria --",'filtra','');
	     		echo obrigatorio();
	     	?>
	     </td>
      </tr>
      <?} ?>
      <tr>
        <td align='right' class="SubTituloDireita">CÛd:</td>
	     <td>
	     	<?=campo_texto('cod','S','S','',7,5,'','');?>
	     </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Nome:</td>
	     <td>
	     	<?=campo_texto('dsc','S','S','',50,250,'','');?>
	     </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Status:</td>
	       <td>
            <input type="radio" name="status" value="A" <?=($status=='A'?"CHECKED":"")?>> Ativo
            <input type="radio" name="status" value="I" <?=($status=='I'?"CHECKED":"")?>> Inativo
        </td>
      </tr>
<? if ($cdtid) { ?>
	<tr bgcolor="#CCCCCC">
   		<td></td>
   		<td><input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao">
   		<input type="button" name="btcancela" value="Cancelar" onclick="inicio();" class="botao"></td>
 	</tr>
<? } else { ?>
	<tr bgcolor="#CCCCCC">
   		<td></td>
   		<td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao">
   		<input type="button" name="btcancela" value="Cancelar" onclick="inicio();" class="botao"></td>
	</tr>
<? } ?>
      
    </table>

  
</form>


<? 
if ($ctbid){
	//exibe listagem das categorias
	$sql= "select 
			'<div align=center><a href=\"monitora.php?modulo=sistema/tabapoio/categorias&acao=A&cdtid=' || c.cdtid || '&op=update&tcbid=".$tcbid."\"><img border=0 src=\"../imagens/alterar.gif\" /></a> <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'monitora.php?modulo=sistema/tabapoio/categorias&acao=A&cdtid=' || c.cdtid || '&act=excluir\');\" ><img border=0 src=\"../imagens/excluir.gif\" /></a>' AS op,
			c.cdtid || ' ' as codigo2,
			c.cdtcod || ' ' as cod,  
			c.cdtdsc as descricao2,
			CASE
				WHEN c.cdtstatus = 'I' THEN '<font color=red>Inativo</font>' 
				WHEN c.cdtstatus = 'A' THEN '<font color=blue>Ativo</font>'
			END as status2
	 		from public.combodadostabela c
	 		Where ctbid = ".$ctbid."
	 		order by cdtdsc ";
	$cabecalho = array("AÁ„o","Id","CÛd","Nome","Status");
	//$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
	$db->monta_lista_simples( $sql, $cabecalho, 300, 10, 'N', '', '');
	
}
?>
 


<script>

function strSpecial(string){
	var search = '·‚‡„¡¬¿√ÈÍË… »ÌÓÏÕŒÃÛıÚı”‘Ã’˙˚˘⁄€Ÿ!@#$%®&*()-+][}{~^¥`?:;,"';

	for (i = 0; i < search.length; i++){
		if ( string.indexOf(search.substr(i,1)) >= 0 ){
			alert('Esse caractere È inv·lido para o campo: " '+search.substr(i,1)+' "');
			return false;
		}		
	}
	return true;
}



function altera_cadastro(cod) {
    document.formulario.sisidEdit.value = cod;
    document.formulario.act.value = 'alterar';
    document.formulario.submit();
}



function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este item?")
	if (questao){
		window.location = url;
	}
}

function inicio() {
 	document.formulario.act.value = '';
 	location.href='monitora.php?modulo=sistema/tabapoio/categorias&acao=A';
}
  
function validar_cadastro(cod) {

	if (document.formulario.ctbid.value == "") 
	{
		alert("Informe o Tipo.");
		document.formulario.ctbid.focus();
		return;
	}
	if (document.formulario.cod.value == "") 
	{
		alert("Informe o CÛd.");
		document.formulario.cod.focus();
		return;
	}
	if (document.formulario.dsc.value == "") 
	{
		alert("Informe o Nome.");
		document.formulario.dsc.focus();
		return;
	}
	if ((document.formulario.status[0].checked == 0) && (document.formulario.status[1].checked == 0)) 
	{
		alert("Informe o Status.");
		document.formulario.status.focus();
		return;
	}
	
	if (cod == 'I') document.formulario.act.value = 'inserir'; else document.formulario.act.value = 'alterar';
	
    document.formulario.submit();

}    

  function filtra(ctbid)
  {
  	location.href="monitora.php?modulo=sistema/tabapoio/categorias&acao=A&ctbid="+ctbid+"&tcbid=<?=$tcbid?>";
  	//document.formulario.submit();
  }
  
    function filtratipo(tcbid)
  {
  	location.href="monitora.php?modulo=sistema/tabapoio/categorias&acao=A&tcbid="+tcbid;
  	//document.formulario.submit();
  }

</script>


