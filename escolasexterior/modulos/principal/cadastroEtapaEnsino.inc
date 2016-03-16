<?php

//verifica session
verificaSessao();



//verifica permissão
$vPermissao = 'N';
$vpflcod = arrayPerfil();
if( in_array(PERFIL_SUPER_USUARIO, $vpflcod) ){
	$vPermissao = 'S';
}


//excluir pessoa
if($_REQUEST['excluir']=='1' && $_REQUEST['etxid']){
	
	//exclui a pessoa
	$sql = "DELETE FROM escolasexterior.etapaensescolaext WHERE etxid = ".$_REQUEST['etxid'];
	$db->executar($sql);
	
	$db->commit();	
	
	print '<script>
			alert("Operação realizada com sucesso!");
			location.href="escolasexterior.php?modulo=principal/cadastroEtapaEnsino&acao=A";
		   </script>';
	exit();
}


//submit
if($_POST){
	
	//verifica se ja existe a etapa
	/*
		$andV = $_POST['etxid'] ? ' and etxid not in ('.$_POST['etxid'].') ' : '';
		$sql = "select count(etxid) as total 
				from escolasexterior.etapaensescolaext 
				where peestatus = 'A' 
				and eexid = ".$_SESSION['eexid']."
				and peenome = '".strtoupper($_POST['peenome'])."'
				$andV";
		$verifica = $db->pegaUm($sql);
		
		if( (int) $verifica > 0 ){
			print "<script>
					alert('Erro: Pessoa já existe!');
					history.back();
			   	  </script>";
			exit();
		}
	*/
	//fim verifica
	
	
	$etxid = $_POST['etxid']; 
		
	if(!$etxid) { //INSERIR
		
		$sql = "INSERT INTO escolasexterior.etapaensescolaext
			(
				eexid,
            	eteid,
            	dcpid,
			  	serid 
	        ) VALUES (
	        	".$_SESSION['eexid'].",
	        	".$_POST['eteid'].",
	        	".$_POST['dcpid'].",
	        	".$_POST['serid']."
	        ) returning etxid";	
		$etxid = $db->pegaUm($sql);
		
		$db->commit();
	
		unset($_POST);
		
		print '<script>
				alert("Operação realizada com sucesso!");
				location.href="escolasexterior.php?modulo=principal/cadastroEtapaEnsino&acao=A";
			   </script>';
		exit();
		
	}
	else{ //ALTERAR 
		
		$sql = "UPDATE 
					escolasexterior.etapaensescolaext
				SET 
					eteid = ".$_POST['eteid'].",
					dcpid = ".$_POST['dcpid'].",
					serid = ".$_POST['serid']."
				WHERE 
					etxid = ".$etxid;
		$db->executar($sql);
		
		$db->commit();
	
		unset($_POST);
		
		print '<script>
				alert("Operação realizada com sucesso!");
				location.href="escolasexterior.php?modulo=principal/cadastroEtapaEnsino&acao=A&etxid='.$etxid.'";
			   </script>';
		exit();
	
	}
	
	
	
}



// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$titulo = "ESCOLAS NO EXTERIOR";

$dsctitulo = 'ETAPA DE ENSINO - DADOS DA DISCIPLINAS<BR><img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';


montaAbasEscolasExterior();

monta_titulo( $titulo, $dsctitulo );

cabecalhoEE($_SESSION['etxid']);


//carrega dados para edição
if($_REQUEST['etxid']){
	$sql = "SELECT  
				  	etxid,
					eteid,
				  	serid,
				  	dcpid
		     FROM 
		     		escolasexterior.etapaensescolaext
		     WHERE 
		     		etxid = {$_REQUEST['etxid']}";
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
}



?>

<script src="../includes/calendario.js"></script>


<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="etxid" value="<?=$etxid?>">


<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
	<tr>
		<td width="32%" class="SubTituloDireita" colspan='2'>Etapa de Ensino:</td>
		<td colspan='2'>
			<?
				$sql_combo = "SELECT
								eteid as codigo,
								etedsc as descricao
							  FROM 
							  	escolasexterior.etapaensino
							  WHERE etstatus='A'
							  ORDER BY 2";			
				$db->monta_combo("eteid", $sql_combo, $vPermissao, "Selecione a Etapa de Ensino", '', '', '', '', 'S', 'peesexo');
			?>
		</td>
		<td align="right" rowspan="7">
			
				<?php
					if($_SESSION['eexid']){
						$docid = criaDocumento( $_SESSION['eexid'] ); 
						if($docid){
							wf_desenhaBarraNavegacao( $docid , array("eexid" => $_SESSION['eexid']) );
						} 
					}
				?>
			
		</td>
	</tr>
	<tr>
		<td width="32%" class="SubTituloDireita" colspan='2'>Disciplinas:</td>
		<td colspan='2'>
			<?
				$sql_combo = "SELECT
								dcpid as codigo,
								dcpdsc as descricao
							  FROM 
							  	escolasexterior.disciplina
							  WHERE dcpstatus='A'
							  ORDER BY 2";			
				$db->monta_combo("dcpid", $sql_combo, $vPermissao, "Selecione a Disciplina", '', '', '', '', 'S', 'dcpid');
			?>
		</td>
	</tr>
	<tr>
		<td width="32%" class="SubTituloDireita" colspan='2'>Série:</td>
		<td colspan='2'>
			<div id="divserie">
			<?
				$sql_combo = " 
					SELECT
						serid as codigo,
						serdsc as descricao
					FROM 
					 	escolasexterior.serie
					WHERE serstatus='A'
					ORDER BY 2 ";
							
				$db->monta_combo("serid", $sql_combo, $vPermissao, "Selecione a Série", '', '', '', '', 'S', 'serid');
			?>
			</div>
		</td>
	</tr>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
							<input type="button" class="botao" name="btSalvar" id="btSalvar" value="Salvar" onclick="validaFormE();">
						<?}?>
						&nbsp;
						<input type="reset" class="botao" name="btLimpar" id="btLimpar" value="Limpar">
						&nbsp;
						<input type="button" class="botao" name="btLimpar" id="btNova" value="Cadastrar Nova Etapa" onclick="location.href='escolasexterior.php?modulo=principal/cadastroEtapaEnsino&acao=A';">
					</td>
					<td align="right">&nbsp;</td>
				</tr>
			</table>
		</td>
	</tr>
</table>

</form>


<?php 
	
	$sql = "SELECT
					'<center>' ||
					(CASE WHEN '{$vPermissao}' = 'S' THEN
							 '<a href=\"#\" onclick=\"alterarItem(\'' || etxid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirItem(\'' || etxid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
						  ELSE
							 '<a href=\"#\"><img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
					END) || '</center>'  as acao,
					etedsc as etapa,
					dcpdsc as disciplina,
	            	serdsc as serie
				FROM escolasexterior.etapaensescolaext e
				INNER JOIN escolasexterior.etapaensino ee on ee.eteid=e.eteid
				INNER JOIN escolasexterior.disciplina d on d.dcpid=e.dcpid
				INNER JOIN escolasexterior.serie s on s.serid=e.serid
				WHERE eexid = {$_SESSION['eexid']}
				order by 2";
		
		$cabecalho 	= array( "Ação","Etapa de Ensino","Disciplina","Série");
		$db->monta_lista_simples( $sql, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
?>

</body>
</html>

 
<script>

var d = document.formulario;



function validaFormE(){

	if(!validaBranco( d.eteid, 'Etapa de Ensino' ))
	{
		return false;
	}
	if(!validaBranco( d.dcpid, 'Disciplina' ))
	{
		return false;
	}
	if(!validaBranco( d.serid, 'Série' ))
	{
		return false;
	}
		
	d.submit();

}


function alterarItem(id){
	location.href='escolasexterior.php?modulo=principal/cadastroEtapaEnsino&acao=A&etxid='+id;
}

function excluirItem(id){
	if(confirm('Deseja realmente excluir este item?')){
		location.href='escolasexterior.php?modulo=principal/cadastroEtapaEnsino&acao=A&excluir=1&etxid='+id;
	}
}


</script>