<?php 

function mensagemAssossiacao($boNomeTipo){
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloCentro" align="center"><font color="red"><?php echo 'É necessário Associar a um '.$boNomeTipo; ?></font></td>
	</tr>
</table>
	<?php
	die;
}

function criaAbaEscolaAtiva($bEstruturaAvaliacao = false){
	
	$abasEscolaAtiva = array();

	if($_SESSION['escolaativa']['boPerfilSuperUsuario']){
		if($bEstruturaAvaliacao){
			// cria a aba e o título da tela
			$abasEscolaAtiva = array( 0 => array( "descricao" => "Lista de Estados",
												  "link"	  => "escolaativa.php?modulo=principal/listaEstados&acao=A" ),
									  1 => array( "descricao" => "Lista de Municípios",
												  "link"	  => "escolaativa.php?modulo=principal/listaMunicipios&acao=A" ),
									  2 => array( "descricao" => "Estrutura Avaliação",
											  	  "link"	  => "escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A" )  
									  );
		} else {
			$abasEscolaAtiva = array( 0 => array( "descricao" => "Lista de Estados",
												  "link"	  => "escolaativa.php?modulo=principal/listaEstados&acao=A" ),
									  1 => array( "descricao" => "Lista de Municípios",
												  "link"	  => "escolaativa.php?modulo=principal/listaMunicipios&acao=A" ) 
									  );
		}	
	} else {
		if($bEstruturaAvaliacao){
			if($_SESSION['escolaativa']['boAbaMunicipio'] && !$_SESSION['escolaativa']['boAbaEstado'] ){
				$abasEscolaAtiva = array( 0 => array( "descricao" => "Lista de Municípios",
													  "link"	  => "escolaativa.php?modulo=principal/listaMunicipios&acao=A" ),
				 						  1 => array( "descricao" => "Estrutura Avaliação",
											  	      "link"	  => "escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A" ) 
										  );
			} elseif($_SESSION['escolaativa']['boAbaEstado'] && !$_SESSION['escolaativa']['boAbaMunicipio']){
				$abasEscolaAtiva = array( 0 => array( "descricao" => "Lista de Estados",
													  "link"	  => "escolaativa.php?modulo=principal/listaEstados&acao=A" ),
				 						  1 => array( "descricao" => "Estrutura Avaliação",
											  	      "link"	  => "escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A" ) 
										  );
			} else {
				$abasEscolaAtiva = array( 0 => array( "descricao" => "Estrutura Avaliação",
											  	      "link"	  => "escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A" ) 
										  );
			}
		} else {
			if($_SESSION['escolaativa']['boAbaMunicipio'] && !$_SESSION['escolaativa']['boAbaEstado'] ){
				$abasEscolaAtiva = array( 0 => array( "descricao" => "Lista de Municípios",
													  "link"	  => "escolaativa.php?modulo=principal/listaMunicipios&acao=A" )
										  );
			} elseif($_SESSION['escolaativa']['boAbaEstado'] && !$_SESSION['escolaativa']['boAbaMunicipio']){
				$abasEscolaAtiva = array( 0 => array( "descricao" => "Lista de Estados",
													  "link"	  => "escolaativa.php?modulo=principal/listaEstados&acao=A" )
										  );
			} elseif($_SESSION['escolaativa']['boPerfilPesquisador']) {
				$abasEscolaAtiva = array( 0 => array( "descricao" => "Lista de Municípios",
													  "link"	  => "escolaativa.php?modulo=principal/listaMunicipios&acao=A" ),
										  1 => array( "descricao" => "Estrutura Avaliação",
												  	  "link"	  => "escolaativa.php?modulo=principal/estruturaAvaliacao&acao=A" )  
										  );
			}			
		}
	}
	
	return $abasEscolaAtiva;						  
}

function pegaQrpid( $tecid, $queid, $monid ){
    global $db;
   
    $sql = "SELECT
                    m.qrpid
            FROM
                    escolaativa.monitoramento m
            INNER JOIN
                    questionario.questionarioresposta q ON q.qrpid = m.qrpid
            WHERE
                    m.tecid = {$tecid} AND
                    m.monid = {$monid} AND
                    q.queid = {$queid}";
    $qrpid = $db->pegaUm( $sql );
   
    if(!$qrpid){
        $sql = "SELECT
        			e.entnome
        		FROM
        			entidade.entidade e
				INNER JOIN escolaativa.tecnico t ON t.entid = e.entid
				WHERE
					t.tecid = ".$tecid;
        $titulo = $db->pegaUm( $sql );
        $arParam = array ( "queid" => $queid, "titulo" => "Escola Ativa (".$titulo.")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        $sql = "UPDATE escolaativa.monitoramento SET tecid = {$tecid}, qrpid = {$qrpid} WHERE monid = ".$monid;
        $db->executar( $sql );
        $db->commit();
    }
    return $qrpid;
}

/*** FUNÇÕES WORKFLOW***/


function eaVerificaEstado( $esdid ){
	
	global $db;
	
	$sql = "SELECT esdid FROM workflow.estadodocumento WHERE esdid = {$esdid}";
	
	return $db->pegaUm( $sql );
	
}

function eaCriarDocumento( $monid ) {
	
	global $db;
	
	$docid = eaPegarDocid( $monid );
	
	if( !$docid ) {
		
		// recupera o tipo do documento
		$tpdid = EA_TIPO_DOCUMENTO;
		
		// descrição do documento
		$docdsc = "Fluxo Escola Ativa - n°" . $monid;
		
		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza pap do EMI
		$sql = "UPDATE
					escolaativa.monitoramento
				SET 
					docid = {$docid} 
				WHERE
					monid = {$monid}";

		$db->executar( $sql );
		//$db->commit();
	}
	
	return $docid;
	
}

function eaPegarDocid( $monid ) {
	
	global $db;
	
	$sql = "SELECT
				docid
			FROM
				escolaativa.monitoramento
			WHERE
			 	monid = " . (integer) $monid;
	
	return (integer) $db->pegaUm( $sql );
	
}

function eaPegarEstadoAtual( $monid ) {
	
	global $db; 
	
	$docid = eaPegarDocid( $monid );
	 
	$sql = "select
				ed.esdid
			from 
				workflow.documento d
			inner join 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;
	
	$estado = (integer) $db->pegaUm( $sql );
	 
	return $estado;
	
}

function possuiPerfil( $pflcods ){

	global $db;

	if ( is_array( $pflcods ) ){
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	} else {
		$pflcods = array( (integer) $pflcods );
	} if ( count( $pflcods ) == 0 ) {
		return false;
	}
	$sql = "select
				count(*)
		from seguranca.perfilusuario
		where
			usucpf = '" . $_SESSION['usucpf'] . "' and
			pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	return $db->pegaUm( $sql ) > 0;
}

function pegaPerfil($usucpf){
	global $db;
	$sql = "SELECT pu.pflcod
			FROM seguranca.perfil AS p 
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE p.sisid = '{$_SESSION['sisid']}'
			AND 
			pu.usucpf = '$usucpf'";


	$pflcod = $db->pegaUm( $sql );
	return $pflcod;
}

function pegaMunicipioAssociado($perfil){
	global $db;

	$sql = "SELECT muncod FROM escolaativa.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = '{$perfil}' ";
	$municipio = $db->carregar($sql);

	if($municipio[0]){
		return $municipio;
	}

	return false;
}

function pegaEstadoAssociado($perfil){
	global $db;

	$sql = "SELECT estuf FROM escolaativa.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = '{$perfil}' ";
	$estado = $db->carregar($sql);

	if($estado[0]){
		return $estado;
	}

	return false;
}

function verificaProfMultiplicador(){
	global $db;
	
	# verifica se usuario logado tem registro em professormultiplicador 
	return $db->pegaUm("SELECT e.entid FROM entidade.entidade e
    						inner join escolaativa.tecnico t on e.entid = t.entid
					  WHERE e.entnumcpfcnpj = '{$_SESSION['usucpf']}'");
}

function pegaTecid(){
	global $db;
	
	# pega tecid do usuario logado 
	return $db->pegaUm("select t.tecid from escolaativa.tecnico t
							inner join entidade.entidade e on e.entid = t.entid
					  	where e.entnumcpfcnpj = '{$_SESSION['usucpf']}'");
}

function existeMunicipio($muncod){
	global $db;
	
	return $db->pegaUm("select count(1) from territorios.municipios where muncod = '{$muncod}'");
}

function existeEstado($estuf){
	global $db;
	
	return $db->pegaUm("select count(1) from territorios.estados where estuf = '{$estuf}'");
}

function arvoreMunicipio($aMunicipio, $aProfessorMultiplicador, $muncod, $aMeses){
	global $db;
	
	$munestdescricao = $aMunicipio['mundescricao'];
	$estdescricao    = $aMunicipio['estdescricao'];
	?>
	<ul id="tree" class="filetree treeview-famfamfam">
		<li><span class="folder"><?php echo $estdescricao; ?></span>
			<ul>
				<li><span class="folder"><?php echo $munestdescricao; ?></span>
					<ul>
						<?php if(count($aProfessorMultiplicador) && $aProfessorMultiplicador[0]): ?>
							<?php foreach($aProfessorMultiplicador as $professorMultiplicador) : ?>
								<li><span><img alt="" src="../imagens/pessoas.png" align="top" border="0" /><?php echo $professorMultiplicador['entnome']; ?></span>
									<?php 
										$sql = "SELECT 
												    e.entnome,
												    ep.espid
												FROM escolaativa.tecnicomonitoraescola tme
												inner join escolaativa.escolaparticipante ep on tme.espid = ep.espid
												inner join cte.escolaativaescolas ea on ep.eaeid = ea.eaeid
												inner join entidade.entidade e on ea.entid = e.entid
												where tme.tecid = {$professorMultiplicador['tecid']} ";
										$arEscolasAssociadas = $db->carregar($sql);
										$arEscolasAssociadas = ($arEscolasAssociadas) ? $arEscolasAssociadas : array();
									?>
									<ul>
									<?php if(count($arEscolasAssociadas) && $arEscolasAssociadas[0]): ?>
										<?php foreach($arEscolasAssociadas as $escola) : ?>
												<li><span class="folder"><?php echo $escola['entnome']; ?></span>
													<!-- Rel Mensal -->
													<?php 
														$sql = "SELECT monid, montipo, qrpid, monreferencia FROM escolaativa.monitoramento where tecid = {$professorMultiplicador['tecid']} and espid = {$escola['espid']} and montipo = '{$_SESSION['escolaativa']['montipo']}' and monreferencia is not null order by monreferencia ";
														$aRelMensal = $db->carregar($sql);
														$aRelMensal = ($aRelMensal) ? $aRelMensal : array();
														
														if($_SESSION['escolaativa']['boPerfilPesquisador']){
										    				$acaoIncluirMensal = "<img src=\"../imagens/gif_inclui_d.gif\" />";	
										    			} else {
										    				$acaoIncluirMensal = "<img src=\"../imagens/gif_inclui.gif\" class=\"abreMensalBimestral\" data-ajax-url=\"escolaativa.php?modulo=principal/popupMensalBimestral&acao=A&monid={$aRelMensal[0]['monid']}&espid={$escola['espid']}&tecid={$professorMultiplicador['tecid']}\" style=\"cursor: pointer\" />";
										    			}
													?>
													<ul>
														<?php if(count($aRelMensal) && $aRelMensal[0]): ?>
															<li><span><img src="../imagens/check_p.gif" border="0" /><?php echo $acaoIncluirMensal; ?> Relatório Mensal </span>
																<ul>
																	<?php foreach ($aRelMensal as $relMensal):?>
																	<?php 
																		/**
																		 * Coloca Imagem WF
																		 */ 
																		$esdid = eaPegarEstadoAtual($relMensal['monid']);
																		if($esdid == WF_EM_ANALISE_SEDUC){
																			$imgWF = "<img src=\"../imagens/refresh2.gif\" title=\"Em análise SEDUC\" /> ";
																		} else {
																			$imgWF = "<img src=\"../imagens/refresh2_01.gif\" /> ";
																		}
																		
																		$monreferencia = $relMensal['monreferencia'];
														    			$acao = "questionario('{$relMensal['monid']}', 'M', '{$professorMultiplicador['tecid']}', 'municipio');";
														    			
														    			$qtQrpid = 0;
														    			if($relMensal['qrpid']){
														    				$qtQrpid = $db->pegaUm("select count(1) from questionario.resposta where qrpid = {$relMensal['qrpid']}");
														    			}
														    			
															    		if($qtQrpid > 3){
															    			$img = "<img src=\"../imagens/check_p.gif\" /> ";
															    		} else {
															    			$img = "";
															    		}
																	?>
																	<li><?php echo $img; ?> <?php echo $imgWF; ?> <a href="#" onclick="<?php echo $acao; ?>"><?php echo $aMeses[$monreferencia]; ?> </a></li>
																	<?php endforeach;?>
																</ul>
															</li>
		    											<?php else :?>
															<li><span><img src="../imagens/check_p_01.gif" border="0" /><?php echo $acaoIncluirMensal; ?> Relatório Mensal</span></li>
														<?php endif; ?>
													<!-- Fim Rel Mensal -->
													</ul>
												</li>
											<?php endforeach;?>
									<?php else: ?>
											<li><span class="file"> Não existe escola associada.</span></li>
									<?php endif; ?>
									</ul>
								</li>
							<?php endforeach;?>
						<?php else: ?>
						<!--<li><span><a href="par.php?modulo=principal/questoesPontuais&acao=A"><img alt="Questões Pontuais" src="../includes/jquery-treeview/images/question.gif" align="top" border="0" /><strong>Questões Pontuais</strong></a></span></li>-->
						<li><span class="file"> Não existe professor multiplicador associado.</span></li>
						<?php endif; ?>
					</ul>
				</li>
			</ul>
		</li>
	</ul> <!-- ul #tree -->
	
	<?php 
}

function arvoreEstado($estdescricao, $aSupervisor, $estuf, $aMeses){
	global $db;
	
	?>
	<ul id="tree" class="filetree treeview-famfamfam">
		<li><span class="folder"><?php echo $estdescricao; ?></span>
			<ul>
				<?php if(count($aSupervisor) && $aSupervisor[0]): ?>
					<?php foreach($aSupervisor as $supervisor) : ?>
						<li><span><img alt="" src="../imagens/pessoas.png" align="top" border="0" /><?php echo $supervisor['entnome']; ?></span>
							<ul>
								<li><span class="folder">Professores</span>
									<?php 
										$sql = "select 
												    distinct
												    t.tecid,
												    e.entid,
												    e.entnome
												from escolaativa.tecnico t
												    inner join entidade.entidade e on t.entid = e.entid    
												    inner join escolaativa.tecnicoresponsabilidade tr on t.tecid = tr.tecid
												    inner join escolaativa.usuarioresponsabilidade ur on e.entnumcpfcnpj = ur.usucpf and ur.rpustatus = 'A'
												    inner join escolaativa.tecnicomonitoratecnico tmt on t.tecid = tmt.tecidmonitorado
												    inner join territorios.municipio m on ur.muncod = tr.muncod
												where 
												m.estuf = '$estuf' and 
												ur.pflcod = ".ESCOLAATIVA_PERFIL_MULTIPLICADOR." and
												tmt.tecidsuper = {$supervisor['tecid']}";
									    $arProfessorAssociados = $db->carregar($sql);
									    $arProfessorAssociados = ($arProfessorAssociados) ? $arProfessorAssociados : array();
									?>
									<ul>
									<?php if(count($arProfessorAssociados) && $arProfessorAssociados[0]): ?>
										<?php foreach($arProfessorAssociados as $professor) : ?>
												<li><span class="folder"><strong><?php echo $professor['entnome']; ?></strong></span> <!-- Nome professores multiplicadores -->
													<?php 
														/*$sql = "SELECT 
																    e.entnome,
																    ep.espid
																FROM escolaativa.tecnicomonitoraescola tme
																inner join escolaativa.escolaparticipante ep on tme.espid = ep.espid
																inner join cte.escolaativaescolas ea on ep.eaeid = ea.eaeid
																inner join entidade.entidade e on ea.entid = e.entid
																where tme.tecid = {$professor['tecid']} ";
														$arEscolasAssociadas = $db->carregar($sql);*/
														
														$sql = "select 
																	e.entnome, 
																	ep.espid 
																from cte.escolaativa ea
															        inner join cte.instrumentounidade inu on inu.inuid = ea.inuid 
															        inner join cte.escolaativaescolas eae on ea.esaid = eae.esaid
															        inner join entidade.entidade e on eae.entid = e.entid
															        inner join escolaativa.escolaparticipante ep on eae.eaeid = ep.eaeid
															        inner join escolaativa.tecnicomonitoraescola tme on ep.espid = tme.espid
															    where 
																	inu.mun_estuf = '$estuf'
																	and tme.tecid = {$professor['tecid']} ";
														$arEscolasAssociadas = $db->carregar($sql);
														$arEscolasAssociadas = ($arEscolasAssociadas) ? $arEscolasAssociadas : array();
													?>
													<ul>
													<?php if(count($arEscolasAssociadas) && $arEscolasAssociadas[0]): ?>
														<?php foreach($arEscolasAssociadas as $escola) : ?>
																<li><span class="folder"><?php echo $escola['entnome']; ?></span>
																	<!-- Rel Mensal -->
																	<?php 
																		$sql = "SELECT monid, montipo, qrpid, monreferencia FROM escolaativa.monitoramento where tecid = {$professor['tecid']} and espid = {$escola['espid']} and montipo = 'M' and monreferencia is not null order by monreferencia ";
																		$aRelMensal = $db->carregar($sql);
																		$aRelMensal = ($aRelMensal) ? $aRelMensal : array();
																		//ver($sql,$aRelMensal);
																	?>
																	<ul>
																		<?php if(count($aRelMensal) && $aRelMensal[0]): ?>
																			<li><span class="folder"> Relatório Mensal </span>
																				<ul>
																					<?php foreach ($aRelMensal as $relMensal):?>
																					<?php
																						/**
																						 * Coloca Imagem WF
																						 */ 
																						$esdid = eaPegarEstadoAtual($relMensal['monid']);
																						if($esdid == WF_EM_ANALISE_SEDUC){
																							$imgWF = "<img src=\"../imagens/refresh2.gif\" title=\"Em análise SEDUC\" /> ";
																							$boEmAnaliseSeduc = true;
																						} else {
																							$imgWF = "<img src=\"../imagens/refresh2_01.gif\" /> ";
																							$boEmAnaliseSeduc = false;
																						}
																						
																						/**
																						 * Ação
																						 */
																						$monreferencia = $relMensal['monreferencia'];
																						if($_SESSION['escolaativa']['boPerfilSuperUsuario'] || ($_SESSION['escolaativa']['boPerfilSupervisor'] && $boEmAnaliseSeduc) ){
																		    				$link = "<a href=\"#\" onclick=\"questionario('{$relMensal['monid']}', 'M', '{$professor['tecid']}', 'estado');\" > {$aMeses[$monreferencia]} </a>";
																						} else {
																							$link = $aMeses[$monreferencia];
																						}
																		    			
																		    			$qtQrpid = 0;
																		    			if($relMensal['qrpid']){
																		    				$qtQrpid = $db->pegaUm("select count(1) from questionario.resposta where qrpid = {$relMensal['qrpid']}");
																		    			}
																		    			
																			    		if($qtQrpid > 3){
																			    			$img = "<img src=\"../imagens/check_p.gif\" /> ";
																			    		} else {
																			    			$img = "";
																			    		}
																					?>
																					<li><span class="file"><?php echo $img; ?> <?php echo $imgWF;?> <?php echo $link; ?> </span> </li>
																					<?php endforeach;?>
																				</ul>
																			</li>
						    											<?php else :?>
																			<li><span class="file">Não possui relatório mensal</span></li>
																		<?php endif; ?>
																	<!-- Fim Rel Mensal -->
																	</ul>
																</li>
															<?php endforeach;?>
													<?php else: ?>
															<li><span class="file"> Não existe escola associada para o estado selecionado.</span></li>
													<?php endif; ?>
													</ul>
												</li>
											<?php endforeach;?>
									<?php else: ?>
											<li><span class="file"> Não existe professor multiplicador associado.</span></li>
									<?php endif; ?>
									</ul>
								</li>
							</ul>
						</li>
					<?php endforeach;?>
				<?php else: ?>
				<!--<li><span><a href="par.php?modulo=principal/questoesPontuais&acao=A"><img alt="Questões Pontuais" src="../includes/jquery-treeview/images/question.gif" align="top" border="0" /><strong>Questões Pontuais</strong></a></span></li>-->
				<li><span class="file"> Não existe supervisor associado.</span></li>
				<?php endif; ?>
			</ul>
		</li>
	</ul> <!-- ul #tree -->
	
	<?php 
}

function arvoreProfessorMultiplicador($aProfessorMultiplicador, $muncod, $aMeses){
	global $db;
	
	?>
	<ul id="tree" class="filetree treeview-famfamfam">
		<?php if(count($aProfessorMultiplicador) && $aProfessorMultiplicador[0]): ?>
			<?php foreach($aProfessorMultiplicador as $professorMultiplicador) : ?>
				<li><span><img alt="" src="../imagens/pessoas.png" align="top" border="0" /><?php echo $professorMultiplicador['entnome']; ?></span>
					<?php
						$sql = "SELECT m.muncod, m.mundescricao FROM escolaativa.tecnicoresponsabilidade tr 
									inner join territorios.municipio m on tr.muncod = m.muncod WHERE tr.tecid = {$professorMultiplicador['tecid']} and m.muncod = '$muncod' "; 
						$aMunicipiosVinculados = $db->carregar($sql);
						$aMunicipiosVinculados = ($aMunicipiosVinculados) ? $aMunicipiosVinculados : array();
					?>
					<ul>
					<?php foreach($aMunicipiosVinculados as $municipiosVinculados) : ?>
						<li><span class="folder"><?php echo $municipiosVinculados['mundescricao']; ?></span>
							<?php 
								$sql = "select e.entnome, ep.espid from cte.escolaativa ea
											inner join cte.instrumentounidade inu on inu.inuid = ea.inuid 
											inner join cte.escolaativaescolas eae on ea.esaid = eae.esaid
											inner join entidade.entidade e on eae.entid = e.entid
											inner join escolaativa.escolaparticipante ep on eae.eaeid = ep.eaeid
											inner join escolaativa.tecnicomonitoraescola tme on ep.espid = tme.espid
										where inu.muncod = '{$municipiosVinculados['muncod']}' and tme.tecid = {$professorMultiplicador['tecid']}";
								$arEscolasAssociadas = $db->carregar($sql);
								$arEscolasAssociadas = ($arEscolasAssociadas) ? $arEscolasAssociadas : array();
							?>
							<ul>
								<?php if(count($arEscolasAssociadas) && $arEscolasAssociadas[0]): ?>
									<?php foreach($arEscolasAssociadas as $escola) : ?>
										<li><span class="folder"><?php echo $escola['entnome']; ?></span>
											<!-- Rel Mensal -->
											<?php 
												$sql = "SELECT monid, montipo, qrpid, monreferencia FROM escolaativa.monitoramento where tecid = {$professorMultiplicador['tecid']} and espid = {$escola['espid']} and montipo = '{$_SESSION['escolaativa']['montipo']}' and monreferencia is not null order by monreferencia ";
												$aRelMensal = $db->carregar($sql);
												$aRelMensal = ($aRelMensal) ? $aRelMensal : array();
												
												if($_SESSION['escolaativa']['boPerfilPesquisador']){
								    				$acaoIncluirMensal = "<img src=\"../imagens/gif_inclui_d.gif\" />";	
								    			} else {
								    				$acaoIncluirMensal = "<img src=\"../imagens/gif_inclui.gif\" class=\"abreMensalBimestral\" data-ajax-url=\"escolaativa.php?modulo=principal/popupMensalBimestral&acao=A&monid={$aRelMensal[0]['monid']}&espid={$escola['espid']}&tecid={$professorMultiplicador['tecid']}\" style=\"cursor: pointer\" />";
								    			}
											?>
											<ul>
												<?php if(count($aRelMensal) && $aRelMensal[0]): ?>
													<li><span><img src="../imagens/check_p.gif" border="0" /><?php echo $acaoIncluirMensal; ?> Relatório Mensal</span>
														<ul>
															<?php foreach ($aRelMensal as $relMensal):?>
															<?php 
																/**
																 * Coloca Imagem WF
																 */ 
																$esdid = eaPegarEstadoAtual($relMensal['monid']);
																if($esdid == WF_EM_ANALISE_SEDUC){
																	$imgWF = "<img src=\"../imagens/refresh2.gif\" title=\"Em análise SEDUC\" /> ";
																} else {
																	$imgWF = "<img src=\"../imagens/refresh2_01.gif\" /> ";
																}
																
																$monreferencia = $relMensal['monreferencia'];
												    			$acao = "questionario('{$relMensal['monid']}', 'M', '{$professorMultiplicador['tecid']}', 'municipio');";
												    			
												    			$qtQrpid = 0;
												    			if($relMensal['qrpid']){
												    				$qtQrpid = $db->pegaUm("select count(1) from questionario.resposta where qrpid = {$relMensal['qrpid']}");
												    			}
												    			
													    		if($qtQrpid > 3){
													    			$img = "<img src=\"../imagens/check_p.gif\" /> ";
													    		} else {
													    			$img = "";
													    		}
													    		
															?>
															<li><?php echo $img; ?> <?php echo $imgWF; ?> <a href="#" onclick="<?php echo $acao; ?>"><?php echo $aMeses[$monreferencia]; ?></a></li>
															<?php endforeach;?>
														</ul>
													</li>
    											<?php else :?>
													<li><span><img src="../imagens/check_p_01.gif" border="0" /><?php echo $acaoIncluirMensal; ?> Relatório Mensal</span></li>
												<?php endif; ?>
											<!-- Fim Rel Mensal -->
											</ul>
										</li>
									<?php endforeach;?>
								<?php else: ?>
									<li><span class="file"> Não existe escola associada.</span></li>
								<?php endif; ?>
							</ul>
						</li>
					<?php endforeach; //fim do foreach municipio ?>
					</ul> <!-- Fim descricao municipio -->
				</li>
			<?php endforeach;?>
		<?php endif; ?>
	</ul> <!-- ul #tree -->	
	<?php 
}

?>