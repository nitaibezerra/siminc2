<?php

function listaReformulacoes($dados) {
	global $db;

	$sql = "SELECT 	'<img border=\"0\" src=\"../imagens/alterar.gif\" onclick=\"abreReformulacao('||pre.preid||');\" style=\"cursor:pointer\" />' as acao, 
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
	
	$sql = "select count(supvid) from obras.obrainfraestrutura o
			inner join obras.supervisao s on o.obrid = s.obrid
			where o.preid = '".$dados['preid']."' 
			and  s.supstatus = 'A'";
	
	$num_vistorias = $db->pegaUm($sql);
	
	if($num_vistorias > 0) {
		die("<script>
				alert('Esta obra não pode ser reformulada, pois existem vistorias cadastradas.');
				window.location='par.php?modulo=principal/listaObras&acao=A';
		      </script>");
	}
	
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
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( $titulo_modulo, '' );

$obPreObraControle = new PreObraControle();
if($_POST['pesquisa']){
	$post = $_POST;	
}

$post['sisid'] = 45;
$post['tooid'] = 6;
$arObras = $obPreObraControle->recuperarListaObras($post);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	$(function() {
	    $('#formulario').submit(function() { 
			selectAllOptions( document.getElementById( 'ptoid' ) );
	    });
	});
	
	function confirmarReformulacao(preid) {
		var conf = confirm('Deseja realmente reformular esta obra?');
		if(conf) {
			window.location='brasilpro.php?modulo=principal/obras/listaObras&acao=A&requisicao=reformularObra&preid='+preid;
		}
	}
	
	function abreReformulacao(preid) {
		return window.open('brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&preid='+preid, 
					   	   'ProInfancia', "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
	}
	
	function exibirReformulacao(preid, obj) {
		var linha  = obj.parentNode.parentNode;
		var tabela = obj.parentNode.parentNode.parentNode;
		if(obj.title == 'abrir') {
		
			obj.title = 'fechar';
			obj.src = '../imagens/menos.gif';
			
			var linhan = tabela.insertRow(linha.rowIndex);
			var col0 = linhan.insertCell(0);
			col0.innerHTML = '&nbsp;';
			var col1 = linhan.insertCell(1);
			col1.colSpan=9;
			
			$.ajax({
				type: "POST",
				url: "brasilpro.php?modulo=principal/obras/listaObras&acao=A",
				data: "requisicao=listaReformulacoes&preid="+preid,
				async: false,
				success: function(msg){
				col1.innerHTML = msg;
			}
			});

		} else {
			obj.title = 'abrir';
			obj.src = '../imagens/mais.gif';
			
			tabela.deleteRow(linha.rowIndex);
		
		}
		
	}

	$(document).ready(function(){

		$('.mostra').click(function(){			
			arDados = this.id.split("_");	

			if(arDados[2] == 194 || arDados[2] == 215){
				var aba = 'analiseEngenheiro'; 
			}else{
				var aba = 'dados';
			}
			
			return window.open('brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid='+arDados[2]+'&ano='+arDados[1]+'&preid='+arDados[0], 
							   '',
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
						<tr>
							<td colspan="2"> Esfera<br>
								<?php 

								$sql = "SELECT DISTINCT
											ptoesfera as codigo,
											CASE
												WHEN ptoesfera = 'E' THEN 'Estadual'
												WHEN ptoesfera = 'M' THEN 'Municipal'
											END as descricao
										FROM 
											obras.pretipoobra
										WHERE
										 	ptostatus = 'A' AND ptoesfera not in ('T')
										ORDER BY
											descricao";
								$db-> monta_checkbox('ptoesfera[]', $sql, $_POST['ptoesfera']);
								?>															
							</td>
						</tr>
						<tr>
							<td colspan="2">
							Grupo de Município<br>
								<?php 

								$sql = "SELECT 
											tpmid as codigo, 
											tpmdsc as descricao
										FROM 
											territorios.tipomunicipio
										WHERE
											tpmstatus = 'A' 
											AND gtmid = 7
										ORDER BY
											descricao  ";
								$tmpid = $_POST['tpmid'];
								$db->monta_combo( "tpmid", $sql, 'S', 'Selecione...', '', '' );
								?>															
							</td>
						</tr>
						<tr>
							<td colspan="2">
							Tipo de Obra<br>
								<?php 

								$sql = "SELECT 
											pto.ptoid as codigo, 
											pto.ptodescricao as descricao 
										FROM 
											obras.pretipoobra pto
										INNER JOIN obras.preobra pre ON pre.ptoid = pto.ptoid 
										WHERE 
											pto.tooid = 6
										ORDER BY 
											2 desc";
								combo_popup('ptoid', $sql, 'Selecione...', "400x400", 0, $_POST['ptoid'], "", (($entcodent)?"N":"S"), false, false, 5, 400,
											null, null, '', '', null, true, false, '', '', Array('descricao'), Array('dsc') );
								?>															
							</td>
						</tr>
						<tr>
							<td colspan="2">
								Situação
								<br/>
								<?php
									$sql = "SELECT 
												esdid as codigo, 
												esddsc as descricao 
											FROM workflow.estadodocumento 
											WHERE tpdid = ".FLUXO_OBRAS_BRASIL_PRO." 
											ORDER BY esdordem";
									
									$esdid = $_POST['esdid'];
									$db->monta_combo( "esdid", $sql, 'S', 'Selecione...', '', '' );
								?>
							</td>
						</tr>
						<tr>
							<td colspan="2">
								Equipe
								<br/>
								<?php
									$sql = "SELECT DISTINCT
												pu.usucpf as codigo, 
												u.usunome as descricao 
											FROM seguranca.perfilusuario pu
											INNER JOIN seguranca.usuario u ON pu.usucpf = u.usucpf 
											WHERE pu.pflcod IN (".BRASIL_PRO_PERFIL_ENGENHEIRO_FNDE.", ".BRASIL_PRO_PERFIL_COORDENADOR_GERAL.", ".BRASIL_PRO_PERFIL_COORDENADOR_TECNICO.")
											ORDER BY u.usunome";

									$usucpf = $_POST['usucpf'];
									$db->monta_combo( "usucpf", $sql, 'S', 'Selecione...', '', '' );
								?>
							</td>
						</tr>
						<tr>
							<td colspan="2">
								Técnicos - análises efetuadas
								<br/>
								<?php
									$sql = "SELECT DISTINCT
												poa.poausucpfinclusao as codigo, 
												u.usunome as descricao 
											FROM seguranca.perfilusuario pu
											INNER JOIN seguranca.usuario u ON pu.usucpf = u.usucpf
											INNER JOIN obras.preobraanalise poa ON poa.poausucpfinclusao = u.usucpf 
											WHERE pu.pflcod IN (".BRASIL_PRO_PERFIL_ENGENHEIRO_FNDE.")
											ORDER BY u.usunome";

									$poausucpfinclusao = $_POST['poausucpfinclusao'];
									$db->monta_combo( "poausucpfinclusao", $sql, 'S', 'Selecione...', '', '' );
								?>
							</td>
						</tr>
						<tr>
							<td colspan="2">
								Ano de Seleção
								<br/>
								<?php
									$sql = "SELECT DISTINCT
												preanoselecao as codigo,
												preanoselecao as descricao
											FROM 
												obras.preobra
											WHERE
												prestatus = 'A' 
												AND preanoselecao > 0 
												AND tooid = 6";

									$preanoselecao = $_POST['preanoselecao'];
									$db->monta_combo( "preanoselecao", $sql, 'S', 'Selecione...', '', '' );
								?>
							</td>
						</tr>
						<tr>
							<td colspan="2">
								Arquivos Corrompidos:
								<?php 

								$sql = "SELECT 
											'0' as codigo, 
											'Nenhum filtro' as descricao 
										UNION ALL
										SELECT 
											'1' as codigo, 
											'Possui arquivos corrompidos' as descricao ";
//								$db-> monta_checkbox('corrompido[]', $sql, $_POST['corrompido']);
								if( possuiPerfil(Array(BRASIL_PRO_PERFIL_SUPER_USUARIO,BRASIL_PRO_PERFIL_COORDENADOR_GERAL)) ){
									echo "<br>";
									$sql .= "UNION ALL
											 SELECT 
												'2' as codigo, 
												'Possui arquivos corrompidos com substituição pendente' as descricao 
											 UNION ALL
											 SELECT 
												'3' as codigo, 
												'Possui arquivos corrompidos, substituídos e com validação pendente' as descricao 
											 UNION ALL
											 SELECT 
												'4' as codigo, 
												'Possui arquivos corrompidos, substituídos e validados' as descricao 
											 UNION ALL
											 SELECT 
												'5' as codigo, 
												'Não possui arquivos corrompidos' as descricao";
//									$db-> monta_checkbox('substituidos[]', $sql, $_POST['substituidos']);
						 		}
						 		$corrompido = $_REQUEST['corrompido'];
						 		$db->monta_radio('corrompido',$sql,'S',1)
								?>	
								<br>														
							</td>
						</tr>
						<tr>
							<td colspan="2">
								Empenho:
								<br />
								<label>
									<input type="checkbox" name="empenho" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['empenho'] == 1 ? 'checked="checked"' : '' ?>/> 
										Com obra empenhada
								</label>		
								<br>												
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