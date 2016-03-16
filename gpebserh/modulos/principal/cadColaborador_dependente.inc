<?
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */

$dpeid = $_SESSION['gpebserh']['dpeid'];
if ( empty( $dpeid ) ){
	die("<script>
			alert('Faltam parametros para acessar esta tela!');
			history.go(-1);
		</script>");
}

$dadosP = $db->pegaLinha("SELECT * FROM gpebserh.dadospessoais WHERE dpeid=" . $dpeid );
extract($dadosP);

function listaDep($dpeid){
	global $db;
	$cabecalho = array("&nbsp;","CPF do Dependente", "Nome do Dependente", "Data de Nascimento");
	$colunaAcoes = "'<center>'|| '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterarDado(\'' || depid || '\');\" title=\"Editar os Dados\" border=\"0\">' || '&nbsp;<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirDependente(' || depid || ',{$dpeid});\"> ' || 
					   	' </center>' as acoes,";
		
	$sql = " SELECT ".$colunaAcoes." depcpf as cpf, depnome as nome, TO_CHAR(depdtnascimento,'dd/mm/YYYY') as nascimento from gpebserh.dependente where depid in 
							(SELECT depid from gpebserh.pessoaldependente where dpeid = ".$dpeid.")";
		//ver(simec_htmlentities($sql),d);
			
	$db->monta_lista($sql,$cabecalho,100,5,'N','center','N','listacolaborador');
}

function salvar(){
	global $db;
	$cpf = str_replace('.','',$_POST['cadcpf']);
	$cpf = str_replace('-','',$cpf);
	
	$sql = "SELECT depid,depstatus FROM gpebserh.dependente where depcpf = '".$cpf."'";
	$dep = $db->pegaLinha($sql);
	if($dep){
		extract($dep);
		//alteração
		if($depstatus == "I"){
		 $valores = "depcpf = '".$cpf."', depstatus = 'A'";
		
			if($_POST['depdtnascimento']){
				
				$dtnascimento = substr($_POST['depdtnascimento'], 6, 4).'-'.substr($_POST['depdtnascimento'], 3, 2).'-'.substr($_POST['depdtnascimento'], 0, 2);
				$valores .= ", depdtnascimento = '".$dtnascimento."'";
			}
			if($_POST['depnome']){
				
			 	$valores .= ", depnome = '".$_POST['depnome']."'"; 
			}
			if($_POST['depnomemae']){
				
			 	$valores .= ", depnomemae = '".$_POST['depnomemae']."'"; 
			}
			$sql = "UPDATE gpebserh.dependente SET ".$valores." where depcpf = '".$cpf."' RETURNING depid";
			//ver($sql,d);
			$depid = $db->pegaUm($sql);
			
			$db->commit();	
			if($_POST['tpvid']){
			 	$valorevinculo = "tpvid = ".$_POST['tpvid'];
				$dpeid = $_POST['dpeidhidden'];
				$sqltpvid = "SELECT depid from gpebserh.pessoaldependente where depid = ".$depid;
				
				if($db->pegaLinha($sqltpvid)){
					$sql = "UPDATE gpebserh.pessoaldependente SET ".$valorevinculo." where dpeid = ".$depid;
					//ver($sql,d);
				}
				else{
					$sql = "INSERT INTO gpebserh.pessoaldependente(depid,dpeid,tpvid  ) VALUES (".$depid.",".$dpeid.",".$_POST['tpvid'].") ";
									
				}
			
			//ver($sql,d);
			}
		}
		else{
//			ver($depid,d);
			$sql = "SELECT dpeid colaborador, depid dependente from gpebserh.pessoaldependente where depid = ".$depid;
			$status = $db->pegaLinha($sql);
			extract($status);
			if($colaborador == $_POST['dpeidhidden']){
				$valores = "depcpf = '".$cpf."', depstatus = 'A'";
			
				if($_POST['depdtnascimento']){
					
					$dtnascimento = substr($_POST['depdtnascimento'], 6, 4).'-'.substr($_POST['depdtnascimento'], 3, 2).'-'.substr($_POST['depdtnascimento'], 0, 2);
					$valores .= ", depdtnascimento = '".$dtnascimento."'";
				}
				if($_POST['depnome']){
					
				 	$valores .= ", depnome = '".$_POST['depnome']."'"; 
				}
				if($_POST['depnomemae']){
					
				 	$valores .= ", depnomemae = '".$_POST['depnomemae']."'"; 
				}
				$sql = "UPDATE gpebserh.dependente SET ".$valores." where depcpf = '".$cpf."' RETURNING depid";
				//ver($sql,d);
				$depid = $db->pegaUm($sql);
				
				$db->commit();	
				if($_POST['tpvid']){
				 	$valorevinculo = "tpvid = ".$_POST['tpvid'];
					$dpeid = $_POST['dpeidhidden'];
					$sqltpvid = "SELECT depid from gpebserh.pessoaldependente where depid = ".$depid;
					
					if($db->pegaLinha($sqltpvid)){
						$sql = "UPDATE gpebserh.pessoaldependente SET ".$valorevinculo." where dpeid = ".$depid;
						//ver($sql,d);
					}
					else{
						$sql = "INSERT INTO gpebserh.pessoaldependente(depid,dpeid,tpvid  ) VALUES (".$depid.",".$dpeid.",".$_POST['tpvid'].") ";
										
					}
				
				//ver($sql,d);
				}
			}else{
				echo "<script>
					alert('Dependente já cadastrado para outro Colaborador!');
					window.location='gpebserh.php?modulo=principal/cadColaborador_dependente&acao=A';
					</script>";
				die;
			}
		}
	}
	else{
		$dpeid = $_POST['dpeidhidden'];	
		
		$dpecpf = $db->pegaUm("SELECT dpecpf FROM gpebserh.dadospessoais WHERE dpeid = ".$dpeid);
//		ver($dpecpf,d);
//		ver($dpeid,$dpecpf);
//		die;
		if($dpecpf == $cpf){
			echo "<script>
					alert('O CPF do Dependente deve ser diferente que o do Colaborador!');
					window.location='gpebserh.php?modulo=principal/cadColaborador_dependente&acao=A';
					</script>";
			die;
		}else{
			$valores = "'".$cpf."'";
			$campo = "depcpf";
		
			if($_POST['depdtnascimento']){
				$dtnascimento = substr($_POST['depdtnascimento'], 6, 4).'-'.substr($_POST['depdtnascimento'], 3, 2).'-'.substr($_POST['depdtnascimento'], 0, 2);
				$valores .= ",'".$dtnascimento."'";
				$campo .= ",depdtnascimento";
			}
			if($_POST['depnome']){
			 	$valores .= ",'".$_POST['depnome']."'"; 
			 	$campo .= ",depnome";
			}
			if($_POST['depnomemae']){
			 	$valores .= ",'".$_POST['depnomemae']."'"; 
			 	$campo .= ",depnomemae";
			}
		
			$sql = "INSERT INTO gpebserh.dependente(".$campo.", depdtinclusao, depstatus) VALUES (".$valores.",NOW(),'A') RETURNING depid";
			
			//ver($sql,d);
			$depid = $db->pegaUm($sql);
			$db->commit();
			if($_POST['tpvid']){
				$dpeid = $_POST['dpeidhidden'];
			 	/*$sql = "SELECT depid from gpebserh.dependente where depcpf = '".$cpf."'";
				$vinculodep = $db->pegaLinha($sql);
				extract($vinculodep);*/
				$camposvinculo = "depid, dpeid, tpvid";
				$valoresvinculo = $depid.", ".$dpeid.", ".$_POST['tpvid'];
			}
			$sql = "INSERT INTO gpebserh.pessoaldependente(".$camposvinculo.") VALUES (".$valoresvinculo.")";
			//ver($sql,d);
		}
	}
		
	//verifico a foto
	//$campos	= array("dpeid" => $cpf);
//	$file = new FilesSimec("dadospessoais", $campos, "gpebserh");
	//ver($_FILES, d);
//	if(is_file($_FILES["foto"]["tmp_name"])){
//		$arquivoSalvo = $file->setUpload($_FILES["foto"]["name"]);
//	}
//		ver($arquivoSalvo, d);
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Operação realizada com sucesso!');
			window.location='gpebserh.php?modulo=principal/cadColaborador_dependente&acao=A';
		  </script>";
	die;
}

if( $_REQUEST['req'] ){
$_REQUEST['req']();

}

//Chamada de programa
if ($_POST['filtraMunicipio'] && $_POST['estuf']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraMunicipioEBS($_POST['estuf']);
	exit;
}

//Buscar CPF
if ($_POST['buscarCpf'] && $_POST['cadcpf']) {
	$cpf = str_replace('.','',$_POST['cadcpf']);
	$cpf = str_replace('-','',$cpf);
	//$sql = "SELECT estufnascimento || '|' || etnid || '|' || carid || '|' || dpenome || '|' || dpedtinclusao || '|' || dpestatus FROM gpebserh.dadospessoais WHERE dpecpf = '".$cpf."'";
	$sql1 ="select  depnome,TO_CHAR(depdtnascimento,'dd/mm/YYYY') depdtnascimento,depnomemae,tpvid from  (SELECT depid as codigo,depnome, depnomemae,depdtnascimento FROM gpebserh.dependente WHERE depcpf = '".$cpf."') dep, gpebserh.pessoaldependente where depid = dep.codigo"; 
/*	$sql = "SELECT dpesiape || '|' || etcid || '|' || nesid || '|' || nacid || '|' || estufnascimento || '|' || paiidorigem || '|' || etnid
			 || '|' || gsgid || '|' || tpdid || '|' || estufctps || '|' || estuflogradouro || '|' || muncodlogradouro || '|' || carid || '|' || arqid
			 || '|' || paiidlogradouro || '|' ||estufrg || '|' || depeditalreg || '|' || dpecpf || '|' || dpenome || '|' || dpesexo || '|' || dpeanochegada
			 || '|' || dpepispasep || '|' || dpenumtitulo || '|' || dpenomemae || '|' || dpefatorrh || '|' || dpeqtddependente || '|' || dpenumrg
			 || '|' || deporgaoexprg || '|' || dpelogradouro || '|' || dpenumlogradouro || '|' || dpecompllogradouro || '|' || dpebairrologradouro || '|' || depceplogradouro
			 || '|' || dpenumddd || '|' || dpenumtelefone || '|' || dpeemail || '|' || dpedtprimemprego || '|' || dpenumbanco || '|' || dpenomeagencia || '|' || dpenumagencia
			 || '|' || dpedigveragencia || '|' || dpecontacorrente || '|' || dpedigverconta || '|' || dpenumctps || '|' || dpenumseriectps || '|' || dpedtnasc FROM gpebserh.dadospessoais 
			 WHERE dpecpf = '".$cpf."'";*/
	
	//$nome = $db->pegaUm($sql);
	$nome = array();
	$nome = $db->pegaLinha($sql1);
	if ($nome){
		echo simec_json_encode($nome);
	}else {
		$sql2 = "SELECT depid as codigo,depnome, depnomemae,TO_CHAR(depdtnascimento,'dd/mm/YYYY')depdtnascimento FROM gpebserh.dependente WHERE depcpf = '".$cpf."'";
		$nome2 = $db->pegaLinha($sql2);
		if( $nome2 ){
		echo simec_json_encode($nome2);
	} else{
		echo 99;
	}
	}
	
	exit;
}

if ($_POST['alterarDep'] && $_POST['depid']) {
	$depid = $_POST['depid'];
	$sql ="select depcpf, depnome,TO_CHAR(depdtnascimento,'dd/mm/YYYY') depdtnascimento,depnomemae,tpvid from  (SELECT depcpf, depid as codigo,depnome, depnomemae,depdtnascimento FROM gpebserh.dependente WHERE depid = '".$depid."') dep, gpebserh.pessoaldependente where depid = dep.codigo"; 
	$nome = array();
	$nome = $db->pegaLinha($sql);
	if( $nome ){
		echo simec_json_encode($nome);
	} else {
		echo 99;
	}
	exit;
}

if( $_POST['deletaDep'] && $_POST['depid'] && $_POST['dpeid'] ){
	$sql = "DELETE FROM gpebserh.pessoaldependente WHERE depid = ".$_POST['depid']."; ";
	$sql .= "UPDATE gpebserh.dependente SET depstatus = 'I' WHERE depid = ".$_POST['depid']."; ";
	
	$db->executar($sql);
	$db->commit();
	
	echo listaDep($where);
	exit;
	
}


include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );
//ver($abacod_tela, $url,$titulo_modulo,d);
?>
<script src="/includes/prototype.js"></script> 
<script src="/includes/entidades.js"></script>
<script src="/includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
 
jQuery.noConflict(); 
 
jQuery(document).ready(function(){

	jQuery('.salvar').click(function(){
		jQuery('#req').val('salvar');
		
		var erro = false;
		jQuery('.obrigatorio').each(function(){
			if( jQuery(this).val() == '' ){
				erro = true;
				jQuery(this).focus();
				return false;
			}
		});
		
		if( erro ){
			alert('Campo obrigarório.');
			return false;
		}
		
		jQuery('#formulario').submit();
	});
	
jQuery('#cadcpf').blur(function(){
		var cpf = jQuery(this).val();
//		alert(cpf);
		//var teste;
     	 //Requisição Ajax
      	jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   dataType: "json", //Tipo de Retorno
			   data: "buscarCpf=true&cadcpf=" + cpf,
        	   success: function(resp){
			  	   	jQuery('#depnome').val(resp.depnome);
			  	   	jQuery('#depnomemae').val(resp.depnomemae);
			  	   	jQuery('#depdtnascimento').val(resp.depdtnascimento);
			  	   	jQuery('#tpvid').val(resp.tpvid);
			  	   	
			  	   	
			}
		});
	});
	//Quando o Documento HTML estiver carregado
	
	
});

function alterarDado(depid){
	jQuery.ajax({
		   type: "POST",
		   url: window.location,
		    dataType: "json", //Tipo de Retorno
		   data: "alterarDep=true&depid=" + depid,
       	   success: function(resp){
		  	   jQuery('#cadcpf').val(resp.depcpf);
		  	   jQuery('#depnome').val(resp.depnome);
		  	   	jQuery('#depnomemae').val(resp.depnomemae);
		  	   	jQuery('#depdtnascimento').val(resp.depdtnascimento);
		  	   	jQuery('#tpvid').val(resp.tpvid);
		}
	});
}

function excluirDependente(depid, dpeid){
	if( confirm("Tem certeza que deseja excluir esse dependente?") ){
		jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   data: "deletaDep=true&depid=" + depid + "&dpeid=" + dpeid,
        	   success: function(resp){
			  		alert('Operação realizada com sucesso!');
			  		jQuery('#listaDependente').html(resp);
			}
		});
	}
}
</script>
<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="req" id="req" value=""/>
	<input type="hidden" name="dpeidhidden" id="dpeidhidden" value="<?=$dpeid ?>"/>
	<table border="0" class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<thead>
			<tr>
				<td class="textosTabela"></td>
				<td class="camposTabela"></td>
				<td class="textosTabela"></td>
				<td></td>
			</tr>
		</thead>
		<tbody>
			<tr>
				<td class="SubTituloDireita">CPF:</td>
				<td colspan="3">
				<?php
					
					echo campo_texto( 'cadcpf', 'S', 'S', '', 20, 80, '###.###.###-##', '','left','',0,'id="cadcpf"','',$cadcpf,""); 
				?>
				</td>
				
			</tr>
		    <tr>
				<td class ="SubTituloDireita" align="right">Nome: </td>
					<td>
						<?= campo_texto('depnome', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="depnome" onblur="MouseBlur(this);"' ); ?>  
					</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right">Data de Nascimento: </td>
				<td>
					<?= campo_data2( 'depdtnascimento','S', 'S', '', 'S' ); ?>  
				</td>
			</tr> 
			
			<tr>
				<td class ="SubTituloDireita" align="right">Nome da Mãe: </td>
				<td>
					<?= campo_texto('depnomemae', 'N', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="depnomemae" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr> 			
			<tr>
				<td class ="SubTituloDireita" align="right">Tipo de Vinculo: </td>
				<td>
					<?php
					$sql = "SELECT tpvid as codigo, tpvdsc as descricao FROM gpebserh.tipovinculo";
					$db->monta_combo("tpvid",$sql,"S","Selecione...",'','','','','S','tpvid');
					?> 
				</td>
			</tr> 			
				<td class ="SubTituloDireita" align="right">  </td>
				<td><input type="button" class="salvar" value="Salvar"></td>
			</tr> 
			<tr>
				<td colspan="3">
					<div id="listaDependente">
					<?=listaDep($dpeid);?>
					</div>
				</td>
			</tr> 
		</tbody>
		
	</table>
</form>