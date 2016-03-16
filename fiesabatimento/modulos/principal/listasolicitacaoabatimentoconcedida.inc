<?
/*
if( !in_array($_SESSION['usucpf'], $cpf_liberados) ){
	echo '<script>
 			location.href = "fiesabatimento.php?modulo=inicio&acao=C";
 		  </script>';
	die;
}
*/

if($_REQUEST['requisicao'])
{
	$_REQUEST['requisicao']($_REQUEST);
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
monta_titulo('Solicitações de Abatimento Confirmadas', $linha2);
?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script>

	jQuery(document).ready(function(){
		jQuery('#aprovar').click(function(){
			if( confirm('Deseja aprovar as solicitações selecionadas?') ){
				jQuery('#formulario_aprovacao').submit();
			}
		});
		jQuery('.mostra').click(function(){
			if( jQuery('#div_'+jQuery(this).attr('id')).css('display') == 'none' ){
				jQuery(this).attr('src','../imagens/menos.gif');
				jQuery('#div_'+jQuery(this).attr('id')).show();
			}else{
				jQuery(this).attr('src','../imagens/mais.gif');
				jQuery('#div_'+jQuery(this).attr('id')).hide();
			}
		});
	});

	function consultarSolicitacao(atpid){
		window.location.href = 'fiesabatimento.php?modulo=principal/aprovacaoabatimentoconcedido&acao=A&atpid=' + atpid;
	}
</script>
<form name="formulario_aprovacao" id="formulario_aprovacao" action="" method="post" >
	<input type="hidden" name="requisicao" value="aprovarSolicitacaoDiretor"/>
	 
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<!--
		<tr>
			<td class="SubTituloDireita" width="25%">Orientações :</td>
			<td>
			<p>Nesta tela são apresentadas as solicitações e renovações de abatimento FIES para aprovação, reabertura ou rejeição pelo Secretário de Educação ou pelo Diretor de Escola Federal, conforme o caso.</p>
			<p>A pesquisa poderá ser iniciada mediante informação de dados pessoais do solicitante (CPF e nome), data da solicitação (início e fim) e as seguintes situações: Em solicitação pelo professor, Pendente de confirmação e Rejeitada por decurso de prazo (a cargo do Diretor de Escola) e Confirmada/Pendente de aprovação, Aprovada, Rejeitada, Reaberta e Rejeitada por decurso de prazo (a cargo do Secretário).</p>
			<p>Assim, preencha o(s) campo(s)  e acione a opção "Pesquisar" ou selecione uma das situações permitidas e acione a opção "Pesquisar".</p>
			<p>Acione o ícone "Detalhar" ao lado de cada registro de solicitação para visualizar os detalhes de cada caso, e realizar a reabertura ou rejeição.<p></p>
			<p>Selecione o(s) registro(s) que queira aprovar, e em seguida acione a opção "Aprovar".</p>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">CPF :</td>
			<td><? echo campo_texto("xx", "N", "S", '', 14, 15, "###.###.###-##", "", "", "", 0, ""); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome do solicitante :</td>
			<td><? echo campo_texto("xx", "N", "S", '', 60, 150, "", "", "", "", 0, ""); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data da solicitação:</td>
			<td>Início: <?php echo campo_data2("dmddatainiexecucao","N","S","","","","",$dmddatainiexecucao); ?> Fim: <?php echo campo_data2("dmddatafimexecucao","N","S","","","","",$dmddatafimexecucao); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação :</td>
			<td><? $db->monta_combo('situacao', array(), 'S', 'Selecione', '', '', '', '200', 'N', 'situacao'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td><input type="button" name="pesquisar" value="Pesquisar"></td>
		</tr>
		-->
		<tr>
			<td colspan="2">
				<br>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<?php 
				$perfis = pegaPerfilGeral();
				
				$where[] = '1=1';
				
	//			if(in_array(PFL_SECRETARIO,$perfis)) {
	//				$esfera = verificaEsferaEntidade();
	//				$where[] = "foo.co_dep_adm = '$esfera'";
	//				if( $esfera = 'M' ){
	//					$muncod = pegaMuncodCPF();
	//					$where[] = "mun.muncod = '$muncod'";
	//				}
	//				if( $esfera = 'E' ){
	//					$estuf = pegaEstufCPF();
	//					$where[] = "mun.estuf = '$estuf'";
	//				}
	//			}
				$sql = "SELECT DISTINCT
							atp.atpdescricaoescola
						FROM
							fiesabatimento.identificacaodocente ido
						LEFT JOIN fiesabatimento.solicitacaoabatimento  sba ON sba.idoid = ido.idoid
						INNER JOIN workflow.documento 					 doc ON doc.docid = sba.docid
						INNER JOIN workflow.estadodocumento 			 esd ON esd.esdid = doc.esdid AND esd.esdid = ".WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL."
						LEFT JOIN fiesabatimento.atuacaoprofissional    atp ON atp.sbaid = sba.sbaid
						INNER JOIN fiesabatimento.responsavelanoatuacao resp ON resp.atpid = atp.atpid
						LEFT JOIN fiesabatimento.historicotramitacao    his ON his.sbaid = sba.sbaid
						LEFT JOIN territorios.municipio 				 mun ON mun.muncod::integer = ido.co_municipio::integer
						/*
						INNER JOIN
							(
								SELECT *
								FROM dblink (
									'dbname=
									hostaddr=
									user=
									password=
									port=5432',
									'SELECT DISTINCT
										co_pessoa_juridica,
										co_dep_adm
									FROM 
										public.tb_sf_pessoa_juridica ;
									'
								) as rs (
									co_pessoa_juridica integer,
									co_dep_adm character(1)
								)
							) as foo ON foo.co_pessoa_juridica = atp.co_pessoa_juridica
							*/
						WHERE
							".implode(' AND ',$where)."
							AND 
							sba.sbastatus = 'A'
							AND atp.atpstatus = 'A'
							AND ido.idostatus = 'A'
							AND to_char(htrdata,'YYYYMM')::integer < ".date('Y')."05
							AND to_char(htrdata,'YYYYMM')::integer > ".(date('Y')-1)."01
							AND sba.sbarenovacao = false
						ORDER BY
							1 ";
				//dbg($sql);
				$dados = $db->carregarColuna($sql);

				echo '<tr><td>';
				echo '<center><font style="font-size: 14;"><b>Lista de Solicitações</b></font></center>';
				echo '</td></tr>';
					
				if($dados){
				
					foreach( $dados as $k => $dado ){
					?>
						<tr>
							<td>
								<img src="../imagens/mais.gif" id="<?=$k ?>" class="mostra" style="cursor:pointer"/>
								<?=$dado ?>
								<div id="div_<?=$k ?>" style="display:none">
									<?php 
									$where[] = "atp.atpdescricaoescola ilike '$dado'";
									$sql = "SELECT DISTINCT
												'<img src=\"../imagens/consultar.gif\" onclick=\"consultarSolicitacao(\'' || atp.atpid || '\')\" style=\"cursor:pointer\" />' as detalhar,
												'<input type=\"checkbox\" name=\"chk_aprovar[]\" value=\"' || ido.idoid || '\" >' as aprovar,
												mun.estuf,
												mun.mundescricao,
												atp.atpdescricaoescola,
												ido.idocpf,
												ido.idonome,
												to_char(htrdata,'DD/MM/YYYY') as dat_solicitacao,
												esd.esddsc as situacao
											FROM
												fiesabatimento.identificacaodocente ido
											LEFT JOIN fiesabatimento.solicitacaoabatimento  sba ON sba.idoid = ido.idoid
											INNER JOIN workflow.documento 					 doc ON doc.docid = sba.docid
											INNER JOIN workflow.estadodocumento 			 esd ON esd.esdid = doc.esdid AND esd.esdid = ".WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL."
											LEFT JOIN fiesabatimento.atuacaoprofissional    atp ON atp.sbaid = sba.sbaid
											INNER JOIN fiesabatimento.responsavelanoatuacao resp ON resp.atpid = atp.atpid
											LEFT JOIN fiesabatimento.historicotramitacao    his ON his.sbaid = sba.sbaid
											LEFT JOIN territorios.municipio 				 mun ON mun.muncod::integer = ido.co_municipio::integer
											/*
											INNER JOIN
												(
													SELECT *
													FROM dblink (
														'dbname=
														hostaddr=
														user=
														password=
														port=5432',
														'SELECT DISTINCT
															co_pessoa_juridica,
															co_dep_adm
														FROM 
															public.tb_sf_pessoa_juridica ;
														'
													) as rs (
														co_pessoa_juridica integer,
														co_dep_adm character(1)
													)
												) as foo ON foo.co_pessoa_juridica = atp.co_pessoa_juridica
												*/
											WHERE
												".implode(' AND ',$where)."
												AND 
												sba.sbastatus = 'A'
												AND atp.atpstatus = 'A'
												AND ido.idostatus = 'A'
												AND to_char(htrdata,'YYYYMM')::integer < ".date('Y')."05
												AND to_char(htrdata,'YYYYMM')::integer > ".(date('Y')-1)."01
											ORDER BY
												1 ";
									$arrCab = array("Deatalhar","Aprovar (Sim / Não)","UF","Município","Nome da Escola","CPF","Nome do solicitante","Data da Solicitação","Situação");
									$db->monta_lista($sql,$arrCab,100,10,"N","center","N");
									
									?>
								</div>
							</td>
						</tr>				
					<?php 	
					}
					
				}else{
					echo '<tr><td>';
					echo '<center><font color="red">Não exitem registros</font></center>';
					echo '</td></tr>';
				}
			 ?>
			 	</table>
			 	 
			 	<br><br>
			 	<!--
			 	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			 	 
			<?php 
			
				$perfis = pegaPerfilGeral();
				
				$where[] = '1=1';
				
				$sql = "SELECT DISTINCT
							atp.atpdescricaoescola
						FROM
							fiesabatimento.identificacaodocente ido
						LEFT JOIN fiesabatimento.solicitacaoabatimento  sba ON sba.idoid = ido.idoid
						INNER JOIN workflow.documento 					 doc ON doc.docid = sba.docid
						INNER JOIN workflow.estadodocumento 			 esd ON esd.esdid = doc.esdid AND esd.esdid = ".WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL."
						LEFT JOIN fiesabatimento.atuacaoprofissional    atp ON atp.sbaid = sba.sbaid
						INNER JOIN fiesabatimento.responsavelanoatuacao resp ON resp.atpid = atp.atpid
						LEFT JOIN fiesabatimento.historicotramitacao    his ON his.sbaid = sba.sbaid
						LEFT JOIN territorios.municipio 				 mun ON mun.muncod::integer = ido.co_municipio::integer
						/*
						INNER JOIN
							(
								SELECT *
								FROM dblink (
									'dbname=
									hostaddr=
									user=
									password=
									port=',
									'SELECT DISTINCT
										co_pessoa_juridica,
										co_dep_adm
									FROM 
										public.tb_sf_pessoa_juridica ;
									'
								) as rs (
									co_pessoa_juridica integer,
									co_dep_adm character(1)
								)
							) as foo ON foo.co_pessoa_juridica = atp.co_pessoa_juridica
							*/
						WHERE
							".implode(' AND ',$where)."
							AND 
							sba.sbastatus = 'A'
							AND atp.atpstatus = 'A'
							AND ido.idostatus = 'A'
							AND to_char(htrdata,'YYYYMM')::integer < ".date('Y')."12
							AND to_char(htrdata,'YYYYMM')::integer > ".(date('Y')-1)."01
							AND sba.sbarenovacao = true
						ORDER BY
							1 ";
				//dbg($sql);
				$dados = $db->carregarColuna($sql);

				echo '<tr><td>';
				echo '<center><font style="font-size: 14;"><b>Lista de Renovações</b></font></center>';
				echo '</td></tr>';
				
				if($dados){
				
					foreach( $dados as $k => $dado ){
					?>
						<tr>
							<td>
								<img src="../imagens/mais.gif" id="<?=$k ?>" class="mostra" style="cursor:pointer"/>
								<?=$dado ?>
								<div id="div_<?=$k ?>" style="display:none">
									<?php 
									$where[] = "atp.atpdescricaoescola ilike '$dado'";
									$sql = "SELECT DISTINCT
												'<img src=\"../imagens/consultar.gif\" onclick=\"consultarSolicitacao(\'' || atp.atpid || '\')\" style=\"cursor:pointer\" />' as detalhar,
												'<input type=\"checkbox\" name=\"chk_aprovar[]\" value=\"' || ido.idoid || '\" >' as aprovar,
												mun.estuf,
												mun.mundescricao,
												atp.atpdescricaoescola,
												ido.idocpf,
												ido.idonome,
												to_char(htrdata,'DD/MM/YYYY') as dat_solicitacao,
												esd.esddsc as situacao
											FROM
												fiesabatimento.identificacaodocente ido
											LEFT JOIN fiesabatimento.solicitacaoabatimento  sba ON sba.idoid = ido.idoid
											INNER JOIN workflow.documento 					 doc ON doc.docid = sba.docid
											INNER JOIN workflow.estadodocumento 			 esd ON esd.esdid = doc.esdid AND esd.esdid = ".WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL."
											LEFT JOIN fiesabatimento.atuacaoprofissional    atp ON atp.sbaid = sba.sbaid
											INNER JOIN fiesabatimento.responsavelanoatuacao resp ON resp.atpid = atp.atpid
											LEFT JOIN fiesabatimento.historicotramitacao    his ON his.sbaid = sba.sbaid
											LEFT JOIN territorios.municipio 				 mun ON mun.muncod::integer = ido.co_municipio::integer
											/*
											INNER JOIN
												(
													SELECT *
													FROM dblink (
														'dbname=
														hostaddr=
														user=simec
														password=
														port=5432',
														'SELECT DISTINCT
															co_pessoa_juridica,
															co_dep_adm
														FROM 
															public.tb_sf_pessoa_juridica ;
														'
													) as rs (
														co_pessoa_juridica integer,
														co_dep_adm character(1)
													)
												) as foo ON foo.co_pessoa_juridica = atp.co_pessoa_juridica
												*/
											WHERE
												".implode(' AND ',$where)."
												AND 
												sba.sbastatus = 'A'
												AND atp.atpstatus = 'A'
												AND ido.idostatus = 'A'
												AND to_char(htrdata,'YYYYMM')::integer < ".date('Y')."05
												AND to_char(htrdata,'YYYYMM')::integer > ".(date('Y')-1)."01
											ORDER BY
												1 ";
									$arrCab = array("Deatalhar","Aprovar (Sim / Não)","UF","Município","Nome da Escola","CPF","Nome do solicitante","Data da Solicitação","Situação");
									$db->monta_lista($sql,$arrCab,100,10,"N","center","N");
									
									?>
								</div>
							</td>
						</tr>				
					<?php 	
					}
					
				}else{
					echo '<tr><td>';
					echo '<center><font color="red">Não exitem registros</font></center>';
					echo '</td></tr>';
				}
			 ?>
			 	 
			 	</table>
			 	-->
			 <input type="button" id="aprovar" style="margin:10px" value="Aprovar">
			</td>
		</tr>
	</table>
</form>
