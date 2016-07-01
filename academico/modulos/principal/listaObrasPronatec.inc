<?php

function listaReformulacoes($dados) {
	global $db;

	$sql = "SELECT 	'<img border=\"0\" src=\"../imagens/alterar.gif\" onclick=\"abreReformulacao('||pre.preid||', '||pre.muncod||');\" style=\"cursor:pointer\" />' as acao, 
					pre.predescricao,
					usu.usunome,
					to_char(pre.predatareformulacao,'dd/mm/YYYY') as predatareformulacao
				FROM obras.preobra pre
				INNER JOIN seguranca.usuario             usu ON usu.usucpf  = pre.preusucpfreformulacao 	
				WHERE pre.preidpai = '".$dados['preid']."'
				ORDER BY pre.predatareformulacao";
	
	$cabecalho = array("&nbsp;","Nome da obra", "Criador da reformulação", "Data dat Reformulação");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function reformularObra($dados) {
	global $db;
	
	include_once APPRAIZ . "includes/workflow.php";
	
	/* RESUMO para COPIAR O QUESTIONARIO
	buscar com preidpai o qrpid (preobraanalise)
	buscar questionario.questionarioresposta com o qrpid do pai.
	buscar questionario.resposta com o qrpidpai
	gerar novo preid -> referenciando o pai obras.preobra
	gerar novo qrpid trocando qrptitulo (questionario.questionarioresposta )
	gerar preobraanalise com qrpid e preid novos
	gerar novas resposta com o qrpid novo
	 */
	$docid = $db->pegaUm("SELECT docid FROM obras.preobra WHERE preid='".$dados['preid']."'");
	$result = wf_alterarEstado( $docid, $aedid = WF_AEDID_APROVADO_ENVIAR_EM_REFORMULACAO, $cmddsc = '', $d = array());
	
	if($result) {
	
		$sql = "UPDATE obras.preobra SET predatareformulacao=NOW(), preusucpfreformulacao='".$_SESSION['usucpf']."' WHERE preid='".$dados['preid']."'";
		$db->executar($sql);
		
		
		// criar novo preid (replicar obras.preobra) 
		$sql = "INSERT INTO obras.preobra(
	            docid, presistema, preidsistema, ptoid, preobservacao, 
	            prelogradouro, precomplemento, estuf, muncod, precep, prelatitude, 
	            prelongitude, predtinclusao, prebairro, preano, qrpid, predescricao, 
	            prenumero, pretipofundacao, prestatus, entcodent, preprioridade, 
	            terid, resid, prevalorobra, tooid, muncodpar, estufpar, premcmv, 
	            preidpai, predatareformulacao, preusucpfreformulacao) 
	            (SELECT NULL, presistema, preidsistema, ptoid, preobservacao, 
	            prelogradouro, precomplemento, estuf, muncod, precep, prelatitude, 
	            prelongitude, predtinclusao, prebairro, preano, NULL, predescricao, 
	            prenumero, pretipofundacao, prestatus, entcodent, preprioridade, 
	            terid, resid, prevalorobra, tooid, muncodpar, estufpar, premcmv, 
	            ".$dados['preid'].", predatareformulacao, preusucpfreformulacao FROM obras.preobra WHERE preid=".$dados['preid'].") RETURNING preid";
		
		$novopreid = $db->pegaUm($sql);
		
		// buscar o qrpid da preobraanalise antiga
		$sql = "SELECT qrpid FROM obras.preobraanalise WHERE preid='".$dados['preid']."'";
		$antigoqrpid = $db->pegaUm($sql);
		
		// buscar o qrpid da preobra2 antiga
		$sql = "SELECT qrpid FROM obras.preobra WHERE preid='".$dados['preid']."'";
		$antigoqrpid2 = $db->pegaUm($sql);
		
		// criar novo qrpid (replicar questionario.questionarioresposta)
		$sql = "INSERT INTO questionario.questionarioresposta(
	            queid, qrptitulo, qrpdata)
	            (SELECT queid, 'OBRAS (".$novopreid.")', qrpdata FROM questionario.questionarioresposta WHERE qrpid='".$antigoqrpid."') RETURNING qrpid";
		$novoqrpid = $db->pegaUm($sql);
		
		// pegando descricao o municipio
		$mundescricao = $db->pegaUm("SELECT m.mundescricao FROM obras.preobra p 
								     LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
								     WHERE preid='".$dados['preid']."'");
		
		// criar novo qrpid (replicar questionario.questionarioresposta)
		$sql = "INSERT INTO questionario.questionarioresposta(
	            queid, qrptitulo, qrpdata)
	            (SELECT queid, 'OBRAS (".$novopreid." - ".$mundescricao.")', qrpdata FROM questionario.questionarioresposta WHERE qrpid='".$antigoqrpid2."') RETURNING qrpid";
		$novoqrpid2 = $db->pegaUm($sql);
		
		
		$db->executar("UPDATE obras.preobra SET qrpid='".$novoqrpid2."' WHERE preid='".$novopreid."'");
		
		
		// criar novo panid (replicar) 
		$sql = "INSERT INTO obras.preobraanalise(
	            preid, poadataanalise, poastatus, poausucpfinclusao, qrpid, poaindeferido, 
	            poajustificativa)
	            (SELECT '".$novopreid."', poadataanalise, poastatus, poausucpfinclusao, ".(($novoqrpid)?"'".$novoqrpid."'":"NULL").", poaindeferido, 
	            poajustificativa FROM obras.preobraanalise WHERE preid='".$dados['preid']."')";
		$db->executar($sql);
		
		$sql = "INSERT INTO questionario.resposta(
	            perid, qrpid, usucpf, itpid, resdsc)
	            (SELECT perid, '".$novoqrpid."', usucpf, itpid, resdsc FROM questionario.resposta WHERE qrpid='".$antigoqrpid."')";
		$db->executar($sql);
		
		$sql = "INSERT INTO questionario.resposta(
	            perid, qrpid, usucpf, itpid, resdsc)
	            (SELECT perid, '".$novoqrpid2."', usucpf, itpid, resdsc FROM questionario.resposta WHERE qrpid='".$antigoqrpid2."')";
		$db->executar($sql);
		
		$sql = "INSERT INTO obras.preobrafotos(
	            pofdescricao, preid, arqid) 
	            (SELECT pofdescricao, '".$novopreid."', arqid FROM obras.preobrafotos WHERE preid='".$dados['preid']."')";
		$db->executar($sql);
		
		$sql = "INSERT INTO obras.preobraanexo(
	            preid, poadescricao, arqid, podid, datainclusao, usucpf, 
	            poasituacao)
	            (SELECT '".$novopreid."', poadescricao, arqid, podid, datainclusao, usucpf, 
	            poasituacao FROM obras.preobraanexo WHERE preid='".$dados['preid']."')";
		$db->executar($sql);
		
		$sql = "INSERT INTO obras.preplanilhaorcamentaria(
	            preid, itcid, ppovalorunitario) 
	            (SELECT '".$novopreid."', itcid, ppovalorunitario FROM obras.preplanilhaorcamentaria WHERE preid='".$dados['preid']."')";
		$db->executar($sql);
		
		$db->commit();
		
		echo "<script>
				alert('Reformulação criada com sucesso');
				window.location='par.php?modulo=principal/listaObras&acao=A';
		      </script>";
	
	} else {
		echo "<script>
				alert('Tramitação não foi possivel. Possivelmente não possui perfil');
				window.location='par.php?modulo=principal/listaObras&acao=A';
		      </script>";
	}
	
	
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}



ini_set( "memory_limit", "1024M" );
set_time_limit(0);

include APPRAIZ."includes/cabecalho.inc";
//include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( $titulo_modulo, 'Pró Infância' );

if($_POST['pesquisa']){
	$post = $_POST;	
}

$arObras = recuperarListaObras($post);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	$(function() {
	    $('#formulario').submit(function() { 
			selectAllOptions( document.getElementById( 'ptoid' ) );
	    });
	});
	
	$(document).ready(function(){

		$('.mostra').click(function(){
			arDados = this.id.split("_");			
			
			return window.open('academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=Dados&preid='+arDados[0]+'&muncod='+arDados[1], 
					   'ProInfancia',
					   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
		});
	});
</script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form method="post" name="formulario" id="formulario">
					<table border="0" cellpadding="3" cellspacing="0">
						<tr>
							<td colspan="2">								
								Nome da obra
								<br/>
								<?php 
									$filtro = simec_htmlentities( $_POST['predescricao'] );
									$predescricao = $filtro;
									echo campo_texto( 'predescricao', 'N', 'S', '', 80, 200, '', ''); 
								?>
							</td>
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								Município
								<br/>
								<?php 
									$filtro = simec_htmlentities( $_POST['municipio'] );
									$municipio = $filtro;
									echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
								?>
							</td>
							<td valign="bottom">
								Estado
								<br/>
								<?php
									$estuf = $_POST['estuf'];
									$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
									$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
								?>
							</td>
						</tr>
					</table>
					<div style="float: left;">
						<input type="submit" name="pesquisar" value="Pesquisar" />
						<input type="hidden" name="pesquisa" value="1>"/>
					</div>
				</form>
			</td>
		</tr>
	</tbody>
</table>
<?php
$arCabecalho = array("Ações","Nome da obra","Tipo da obra","Município","UF","Situação","Usuário", "Data da Situação", "Analista", "Resolução"); 
$db->monta_lista_array($arObras, $arCabecalho, 20, 10, 'N', '');
?>