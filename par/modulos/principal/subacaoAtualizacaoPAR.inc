<?php
if( empty($_SESSION['par']['inuid']) ){
	echo "<script>
				alert('Falta dados da seção!');
				location.href='par.php?modulo=principal/listaMunicipios&acao=A';
	 		</script>";
	exit;
}

if( $_POST['requisicao'] == 'Salvar' ){
	
	$inuid = $_SESSION['par']['inuid'];
	
	// Se estiver alterando
	if( is_array( $_POST['alt'] ) ){
		foreach( $_POST['alt'] as $subacaoAlterada ){
			
			$sql = "SELECT 
						s.docid, aed.aedid, d.esdid
					FROM 
						par.subacao s 
					INNER JOIN workflow.documento d ON d.docid = s.docid 
					LEFT  JOIN workflow.acaoestadodoc aed ON aed.esdidorigem = d.esdid AND esdiddestino = ".WF_SUBACAO_ATUALIZACAO."
					WHERE 
						s.sbaid = ".$subacaoAlterada;
			
			$dadosDocumento = $db->pegaLinha($sql);
			
			$aedid = $dadosDocumento['aedid'];
			
			if( empty($dadosDocumento['esdid']) ){
				$esdiddestino = WF_SUBACAO_ATUALIZACAO;
				
				$sql = "SELECT aeddscrealizar, aeddscrealizada FROM workflow.acaoestadodoc WHERE aedstatus = 'A' and esdiddestino = $esdiddestino limit 1";
				$arAcoes = $db->pegaLinha( $sql );
			
				$sql = "INSERT INTO workflow.acaoestadodoc(
							esdidorigem, esdiddestino, aeddscrealizar, aeddscrealizada,
							aedcondicao, aedobs, aedposacao, aedvisivel, aedicone, aedcodicaonegativa)
						VALUES (
							{$dadosDocumento['esdid']}, $esdiddestino, '{$arAcoes['aeddscrealizar']}', '{$arAcoes['aeddscrealizada']}',
							'', '', '', false, '', false) 
						RETURNING aedid";
			
				$aedid = $db->pegaUm( $sql );
			}
			
			// cria log no histórico
			$sqlHistoricoSub = "INSERT INTO workflow.historicodocumento( aedid, docid, usucpf, htddata )
								VALUES ( " . $aedid . ", " . $dadosDocumento['docid'] . ", '" . $_SESSION['usucpf'] . "', now() )";

			$db->executar($sqlHistoricoSub);

			// atualiza documento
			$sqlDocumentoSub = "UPDATE workflow.documento SET 
									esdid = " . WF_SUBACAO_ATUALIZACAO . ", 
									docdsc = 'Em Atualização'
								WHERE 
									docid = " . $dadosDocumento['docid'];

			$db->executar($sqlDocumentoSub);
		}
		$db->commit();
	} 
	
	// Se estiver adicionando
	if( is_array( $_POST['add'] ) ){
		
		foreach( $_POST['add'] as $ppsid ){
			
			$indid = $db->pegaUm("SELECT indid FROM par.propostasubacao WHERE ppsid = ".$ppsid);
			
			$sql = "select
						a.aciid
					from
						par.indicador i
					inner join par.criterio c ON c.indid = i.indid AND c.crtstatus = 'A'
					inner join par.pontuacao p ON p.crtid = c.crtid AND p.ptostatus = 'A'
					inner join par.acao a ON p.ptoid = a.ptoid AND a.acistatus = 'A'
					where 
						i.indid = {$indid} AND p.inuid = ".$inuid;
			
			$aciid = $db->pegaUm( $sql );

			if( !$aciid ){
				//Adiciono a ação
				
				/*$sql = "SELECT 
							pa.ppaid, pa.ppadsc, p.ptoid
						FROM par.indicador i
							inner join par.criterio c ON c.indid = i.indid AND c.crtstatus = 'A'
							inner join par.pontuacao p ON p.crtid = c.crtid AND p.ptostatus = 'A'
							inner join par.propostaacao pa ON pa.crtid = c.crtid
						WHERE 
							i.indid = {$indid} 
							and p.inuid = {$inuid}";*/
				
				$sql = "select 
							ppa.ppaid, 
							p.ptoid 
						from 
							par.propostasubacao pps
						inner join par.criterio c on c.indid = pps.indid
						inner join par.propostaacao ppa on ppa.crtid = c.crtid
						inner join par.pontuacao p on p.crtid = ppa.crtid and p.inuid = {$inuid} AND p.ptostatus = 'A'
						WHERE
							pps.ppsid = ".$ppsid;
				
				$dadosAcao = $db->pegaLinha($sql);

				$dadosPropostaAcao = $db->pegaLinha( "select * from par.propostaacao WHERE ppaid = ".$dadosAcao['ppaid'] );
				 
				$oAcao 			= new Acao();
				
				$oAcao->ptoid		= $dadosAcao['ptoid']; 
				$oAcao->acidsc		= $dadosPropostaAcao['ppadsc'];
			    $oAcao->acistatus	= 'A';
			    $oAcao->acidata		= 'NOW()';
			    $oAcao->usucpf		= $_SESSION['usucpf'];
				$aciid = $oAcao->salvar();
				$oAcao->commit();
			}
			
			// Salva subação	
			$oSubacao = new Subacao();
			
			$desc = 'Em Atualização';
			$esdid = WF_SUBACAO_ATUALIZACAO;
			
			$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
					VALUES (62, ".$esdid.", '".$desc."', now()) returning docid ";
			$docid = $db->pegaUm($sql);
			
			$propostaSubacao = $db->pegaLinha( "SELECT * FROM par.propostasubacao WHERE ppsid = ".$ppsid );
			
			$oSubacao->sbaid 					  = null;
			$oSubacao->sbadsc 					  = $propostaSubacao['ppsdsc'];
			$oSubacao->sbaordem 				  = $propostaSubacao['ppsordem'];
			$oSubacao->sbaobra 					  = $propostaSubacao['sbaobra'];
			$oSubacao->sbaestrategiaimplementacao = $propostaSubacao['ppsestrategiaimplementacao'];
			$oSubacao->sbaptres 				  = $propostaSubacao['ppsptres'];
			$oSubacao->sbanaturezadespesa 		  = $propostaSubacao['ppsnaturezadespesa'];
			$oSubacao->sbamonitoratecnico 		  = $propostaSubacao['ppsmonitora'];
			$oSubacao->frmid 					  = $propostaSubacao['frmid'];
			$oSubacao->ptsid 					  = $propostaSubacao['ptsid'];
			$oSubacao->sbastatus				  = 'A';
			$oSubacao->indid 					  = $propostaSubacao['indid'];
			$oSubacao->foaid 					  = $propostaSubacao['foaid'];
			$oSubacao->undid 					  = $propostaSubacao['undid'];
			$oSubacao->docid 					  = $docid;
			$oSubacao->ppsid 					  = $propostaSubacao['ppsid'];
			$oSubacao->sbacronograma 			  = $propostaSubacao['ppscronograma'];
			$oSubacao->aciid 					  = $aciid;
			$oSubacao->salvar();
			$oSubacao->commit();

		}
		
	}
	echo "<script>
				alert('Operação realizada com sucesso!');
				location.href='par.php?modulo=principal/subacaoAtualizacaoPAR&acao=A';
	 		</script>";
	exit;
	
}
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

//$db->cria_aba( $abacod_tela, $url, '' );
echo cabecalho();
echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore">';
				echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']);
			echo '</a>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Atualização do PAR', '' );

$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
$mostra = '';

if(	in_array(PAR_PERFIL_PREFEITO,$arrayPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrayPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrayPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrayPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrayPerfil) ||
	in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || 
	in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)
){
	$mostra = 'S';
} else {
	$disabled = "disabled";
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
function listarSubacao(sbaid){
	var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid;
	janela(local,800,600,"Subação");
}

function salvar(){
	if( confirm("Tem certeza que deseja fazer essas alterações?") ){
		$('#requisicao').val("Salvar");
		$('#formatualizacao').submit();
	}
}
function incluirSubacao(){
	url = 'par.php?modulo=principal/incluirSubacaoAtualizacaoPar&acao=A';
	window.open(url, 'popupIncluirSubacaoAtualizacao', "height=600,width=800,scrollbars=yes,top=50,left=200" );
}
</script>
<form name="formatualizacao" id="formatualizacao" method="post" action="">
	<input type="hidden" id="requisicao" name="requisicao" value="">
	<table style="background-color: #F5F5F5;" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<tr>
		<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')"><b>Subações a serem alteradas</b></td>
	</tr>
	<tr>
		<td>
			<div id="Layer1" style="width:100%; height:200; overflow: auto;">
			<table>
				<?php
					//Lista de subações que podem ser alteradas
					$sql = "SELECT
								s.sbaid, d.dimcod || '.' || ar.arecod || '.' || i.indcod || '.' || s.sbaordem || ' - ' || s.sbadsc as subacao
							FROM
								par.subacao s
							INNER JOIN par.acao a ON a.aciid = s.aciid AND a.acistatus = 'A'
							INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
							INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
							INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A' AND c.crtpontuacao IN (1,2,3,4,5) 
							INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
							INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
							INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
							INNER JOIN workflow.documento dd ON dd.docid = s.docid  
							WHERE
								CASE WHEN s.ppsid IS NOT NULL THEN s.ppsid NOT IN (913, 924, 906, 925, 904, 1074, 1075) ELSE 1=1 END AND
								s.sbaextraordinaria IS NULL AND
								s.frmid NOT IN ( 14, 15 ) AND
								s.sbastatus = 'A' AND dd.esdid <> ".WF_SUBACAO_ATUALIZACAO." and 
								p.inuid = {$_SESSION['par']['inuid']} AND
								s.sbaid NOT IN ( SELECT 
													sbaid
												FROM
													( 
													-- Subações empenhadas
													SELECT
														sbaid 
													FROM 
														par.empenhosubacao ems 
													INNER JOIN par.empenho emp ON emp.empid = ems.empid AND emp.empsituacao = '2 - EFETIVADO' and eobstatus = 'A' and empstatus = 'A' 
													WHERE 
														ems.eobano >= (".date("Y").")
														/*
													UNION ALL

													-- Subações em atualização
													SELECT
														sbaid
													FROM
														par.subacao s
													INNER JOIN workflow.documento d ON d.docid = s.docid AND d.esdid = ".WF_SUBACAO_ATUALIZACAO." */ ) as foo )
							ORDER BY
								d.dimcod, ar.arecod, i.indcod, s.sbaordem";

					$dadosSubacaoAlt = $db->carregar($sql);
					
					if(is_array($dadosSubacaoAlt)){
						foreach( $dadosSubacaoAlt as $subacaoAlt ){
							echo '<tr><td nowrap><input type="checkbox" '.$disabled.' name="alt[]" id="alt[]" value="'.$subacaoAlt['sbaid'].'">
										<img style="cursor:pointer" title="Visualizar Subação" src="/imagens/editar_nome.gif" onclick="javascript:listarSubacao(\''.$subacaoAlt['sbaid'].'\')" >
									</td><td>'.$subacaoAlt['subacao'].'</td></tr>';
						}
					}
				?>
			</table>
			</div>
		</td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')"><b>Subações a serem adicionadas</b></td>
	</tr>
	<tr>
		<td>
			<div id="Layer1" style="width:100%; height:200; overflow: auto;">
			<table>
			<?php
				
				//Lista de subações que podem ser incluidas
				$sql = "SELECT 
							pps.ppsid, d.dimcod || '.' || ar.arecod || '.' || i.indcod || '.' || pps.ppsordem || ' - ' || pps.ppsdsc as subacao
						FROM
							par.pontuacao p
						INNER JOIN par.instrumentounidade 	iu ON iu.inuid = p.inuid
						INNER JOIN par.criterio  			c  ON c.crtid  = p.crtid AND c.crtstatus = 'A' AND c.crtpontuacao IN (1,2,3,4,5) 
						INNER JOIN par.indicador 			i  ON i.indid  = c.indid AND i.indstatus = 'A'
						INNER JOIN par.propostasubacao 		pps ON pps.indid = i.indid
						inner join par.propostaacao 		pa on pa.crtid = c.crtid
						INNER JOIN par.area 	 			ar ON ar.areid = i.areid AND ar.arestatus = 'A'
						INNER JOIN par.dimensao  			d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
						WHERE
							p.ptostatus = 'A' AND
							p.inuid = {$_SESSION['par']['inuid']} and
							pps.frmid NOT IN ( 14, 15 ) AND
							pps.ppsid NOT IN (1074, 1075) AND
							pps.ppsid NOT IN ( SELECT
													s.ppsid
												FROM
													par.subacao s
												INNER JOIN par.acao a ON a.aciid = s.aciid AND a.acistatus = 'A'
												INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
												INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
												INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A' 
												INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
												INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
												INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
												WHERE
													s.sbastatus = 'A' AND
													ppsid IS NOT NULL AND
													p.inuid = {$_SESSION['par']['inuid']}
												UNION ALL
												SELECT
													s.ppsid
												FROM
													par.subacao s
												INNER JOIN par.acao a ON a.aciid = s.aciid AND a.acistatus = 'A'
												INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
												WHERE
													s.ppsid NOT IN (913, 924, 906, 925, 904) AND
													s.sbaextraordinaria IS NULL AND
													s.sbastatus = 'A' and
													p.inuid = {$_SESSION['par']['inuid']})
						ORDER BY
							d.dimcod, ar.arecod, i.indcod, pps.ppsordem";

					$dadosSubacaoAdd = $db->carregar($sql);

					if(is_array($dadosSubacaoAdd)){
						foreach( $dadosSubacaoAdd as $subacaoAdd ){
							echo '<tr><td><input type="checkbox" '.$disabled.' name="add[]" id="add[]" value="'.$subacaoAdd['ppsid'].'"></td><td>'.$subacaoAdd['subacao'].'</td></tr>';
						}
					}
			?>
			</table>
			</div>
		</td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9">
				<? 
				$boGrande = recuperaGrandesMunicipios($_SESSION['par']['inuid']);
				if($boGrande || $_SESSION['par']['itrid'] == 1){ 
				?>
					<img border="0" src="../imagens/gif_inclui.gif" align="absmiddle" style="cursor:pointer;" title="Incluir Subação"><a class="" onclick="incluirSubacao()"> <b>Criar uma nova Subação</b> </a>
				<?} ?>
		</td>
	</tr>
	<?php if( $mostra == 'S' ){ ?>
	<tr>
		<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')"><center><input type="button" value="Salvar" onclick="salvar()"></center></td>
	</tr>
	<?php }  ?>
	</table>
</form>
		</td>
	</tr>
</table>