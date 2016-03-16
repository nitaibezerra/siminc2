<?php
	
class MonitoramentoInterno extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.monitoramentointerno";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "mniid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'mniid' => null, 
									  	'inuid' => null, 
									  	'ppsid' => null, 
									  	'mniano' => null, 
									  	'mniqtd' => null,
    									'usucpf' => null,
    									'usucpfvalida' => null,
    									'mnidtvalidacao' => null, 
									  );
									  
	public function montarFormularioGlobalSemCPFPorAno( $sbaid ){

		$sql = "select  distinct spt.sptid, coalesce( spt.sptunt, 0 ) as sptunt,
						coalesce( 
							(select coalesce(es.exeqtd, 0) as exeqtd
							from cte.monitoramentosubacao m
								inner join cte.execucaosubacao es on es.mntid = m.mntid
								inner join cte.periodoreferencia pr on pr.perid = es.perid
							where m.sbaid = s.sbaid and pr.perano = ".$this->mniano." order by pr.perid desc limit 1), 0
						) as exeqtd
				from cte.subacaoindicador s
					left join cte.subacaoparecertecnico spt on spt.sbaid = s.sbaid and spt.sptano = {$this->mniano}
				where s.sbaid = '$sbaid'";
		
		$arDados = $this->carregar( $sql );
		$arDados = $arDados ? array_shift( $arDados ) : array( 'sptunt' => '0', 'exeqtd' => '0' );
		
		/**************************
		 * SQL PROGRAMA POR ESCOLA
		 */
		$sqlQtdPrograma = "	select count( mie.entid ) as qtdEscolas, sum( mie.mneqtd ) as qtdGeral
							from cte.monitoramentointerno mi
								inner join cte.monitoramentointernoentidade mie on mie.mniid = mi.mniid
								inner join entidade.entidade e on e.entid = mie.entid
							where mi.inuid = '{$this->inuid}'
							and mi.ppsid = '{$this->ppsid}'
							and mi.mnidtvalidacao is not null
							and mi.mniano = '{$this->mniano}'";
						
		$arQtdPrograma = $this->pegaLinha( $sqlQtdPrograma ); 						
		$arQtdPrograma = $arQtdPrograma ? $arQtdPrograma : array( 'qtdEscolas' => '0', 'qtdGeral' => '0' ); 					
		
		$sqlPrograma = "select distinct e.entcodent, e.entnome, coalesce( mie.mneqtd, 0 )
						from cte.monitoramentointerno mi
							inner join cte.monitoramentointernoentidade mie on mie.mniid = mi.mniid
							inner join entidade.entidade e on e.entid = mie.entid
						where mi.inuid = '{$this->inuid}'
						and mi.ppsid = '{$this->ppsid}'
						and mi.mniano = '{$this->mniano}'
						order by e.entnome";
		/*
		 * ************************
		 */						
		
		$perfis = cte_arrayPerfil();
		for($x=0; $x<count($perfis); $x++){
			//172, 167, 125*, 288
			if($perfis[$x] == CTE_PERFIL_EQUIPE_TECNICA){
				$equipe = true;
			}else{
				$equipe = false;
			}
		}
		
		if($equipe == false){
			echo '		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
							<tr  class="title" style="background: #e3e3e3; text-align: center;">
								<td width="33%">PAR</td>
								<td width="33%">Monitoramento</td>
								<td width="34%">Programa</td>
							</tr>
							<tr align="center">
								<td>'.$arDados['sptunt'].'</td>
								<td>'.$arDados['exeqtd'].'</td>
								<td>'.campo_texto("mniqtd_temp", 'N', 'N', '', '10', '', '[#]', '', '', '', '', 'id="mniqtd"', '', $this->mniqtd ).'</td>
							</tr>
							<tr>
								<td colspan="3">
									<div>
										<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
											<tr>
												<td align="center">
													<b>Programa</b><br />
													Escolas: '.$arQtdPrograma['qtdescolas'].'<br />  
													Quantidade: '.$arQtdPrograma['qtdgeral'].'<br />  
												</td>
											</tr>
											<tr>
												<td>';												
													$this->monta_lista( $sqlPrograma, array( "Código INEP", "Escola", "Qtd" ), 20, 10, 'N', '', '' );													
			echo '								</td>
											</tr>
										</table>
									</div>
									<div style="padding: 5px 0px 5px 15px;">
										<span id="adEscolas" name="adEscolas" style="cursor:pointer;display:block;float:left" onclick="return popupSelecionarEscolas( \''.$this->mniano.'\', \''.$this->inuid.'\', \''.$this->ppsid.'\' );"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Editar / Inserir Escolas</span>
									</div><br />
								</td>
							</tr>							
							<tr bgcolor="#e3e3e3" align="center">
								<td colspan="3">
									<form action="" method="post" name="formulario_'.$this->mniano.'" id="formulario_'.$this->mniano.'">
										<div>
											<input type="hidden" name="formulario" value="1"/>											
											<input type="hidden" name="mniqtd"  value="'.$this->mniqtd.'"/>
											<input type="hidden" name="inuid"  value="'.$this->inuid.'"/>
											<input type="hidden" name="ppsid"  value="'.$this->ppsid.'"/>
											<input type="hidden" name="mniano" value="'.$this->mniano.'"/>
											<input type="button" class="botao" name="cadastra" value="Salvar" onclick="validarFormulario( \'formulario_'.$this->mniano.'\' );"/>
											<input type="button" class="botao" name="fecha" value="Voltar" onclick="fechar();"/>
										</div>
									</form>
								</td>
							</tr>							
						</table>';
		} else {
			echo '		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
						<!--
							<tr  class="title" style="background: #e3e3e3; text-align: center;">								
								<td width="34%">Programa</td>
							</tr>
							<tr align="center">								
								<td>'.campo_texto("mniqtd_temp", 'N', 'N', '', '10', '', '[#]', '', '', '', '', 'id="mniqtd"', '', $this->mniqtd ).'</td>
							</tr>
						-->
							<tr>
								<td colspan="3">
									<div>
										<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
											<tr>
												<td align="center">
													<b>Programa</b><br />
													Escolas: '.$arQtdPrograma['qtdescolas'].'<br />  
													Quantidade: '.$arQtdPrograma['qtdgeral'].'<br />  
												</td>
											</tr>
											<tr>
												<td>';												
													$this->monta_lista( $sqlPrograma, array( "Código INEP", "Escola", "Qtd" ), 20, 10, 'N', '', '' );													
			echo '								</td>
											</tr>
										</table>
									</div>
									<div style="padding: 5px 0px 5px 15px;">
										<span id="adEscolas" name="adEscolas" style="cursor:pointer;display:block;float:left" onclick="return popupSelecionarEscolas( \''.$this->mniano.'\', \''.$this->inuid.'\', \''.$this->ppsid.'\' );"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Editar / Inserir Escolas</span>
									</div><br />
								</td>
							</tr>							
							<tr bgcolor="#e3e3e3" align="center">
								<td colspan="3">
									<form action="" method="post" name="formulario_'.$this->mniano.'" id="formulario_'.$this->mniano.'">
										<div>
											<input type="hidden" name="formulario" value="1"/>											
											<input type="hidden" name="mniqtd"  value="'.$this->mniqtd.'"/>
											<input type="hidden" name="inuid"  value="'.$this->inuid.'"/>
											<input type="hidden" name="ppsid"  value="'.$this->ppsid.'"/>
											<input type="hidden" name="mniano" value="'.$this->mniano.'"/>
											<input type="button" class="botao" name="cadastra" value="Salvar" onclick="validarFormulario( \'formulario_'.$this->mniano.'\' );"/>
											<input type="button" class="botao" name="fecha" value="Voltar" onclick="fechar();"/>
										</div>
									</form>
								</td>
							</tr>							
						</table>';
		}

	}
									  
	public function montarFormularioGlobalComCPFPorAno( $sbaid ){
		
		/***********************************************************************************
		*									CURSISTAS PAR								   *
		***********************************************************************************/
		
		$sqlPar = " select distinct e.entnumcpfcnpj, e.entnome
					from cte.composicaopessoa cp
						inner join entidade.entidade e on e.entid = cp.entid
					where cp.sbaid = '$sbaid'
					and cspano = '{$this->mniano}'
					order by e.entnome";

		$arPar = $this->carregar( $sqlPar ); 						
		$arPar = $arPar ? $arPar : array();
		
		/***********************************************************************************
		*								CURSISTAS MONITORAMENTO						   	   *
		***********************************************************************************/		
		
		$sqlMonitoramento = "	select distinct ees.eescpf, ees.eesnome, scu.scudsc
								from cte.entidadeexecucaosubacao ees
									inner join cte.execucaosubacao es on es.exeid = ees.exeid
									inner join cte.monitoramentosubacao m on m.mntid = es.mntid
									left  join cte.situacaocursista scu on scu.scuid = ees.scuid
								where m.sbaid = '$sbaid'
								and es.exeid = (
									select coalesce(es.exeid, 0) as exeqtd
									from cte.monitoramentosubacao m
										inner join cte.execucaosubacao es on es.mntid = m.mntid
										inner join cte.periodoreferencia pr on pr.perid = es.perid
									where m.sbaid = $sbaid and pr.perano = '{$this->mniano}' order by pr.perid desc limit 1
								)
								order by ees.eesnome";
		
		$arMonitoramento = $this->carregar( $sqlMonitoramento ); 						
		$arMonitoramento = $arMonitoramento ? $arMonitoramento : array();		
		
		/***********************************************************************************
		*								CURSISTAS PROGRAMA							   	   *
		***********************************************************************************/
		
		$sqlPrograma = "select distinct e.entnumcpfcnpj, e.entnome, scu.scudsc
						from cte.monitoramentointerno mi
							inner join cte.monitoramentointernoentidade mie on mie.mniid = mi.mniid
							inner join entidade.entidade e on e.entid = mie.entid
							left  join cte.situacaocursista scu on scu.scuid = mie.scuid 
						where mi.inuid = '{$this->inuid}'
						and mi.ppsid = '{$this->ppsid}'
						and mi.mniano = '{$this->mniano}'
						and mi.mnidtvalidacao is not null
						order by e.entnome";
		
		$arPrograma = $this->carregar( $sqlPrograma ); 						
		$arPrograma = $arPrograma ? $arPrograma : array();	
		
		$perfis = cte_arrayPerfil();
		for($x=0; $x<count($perfis); $x++){
			//172, 167, 125*, 288
			if($perfis[$x] == CTE_PERFIL_EQUIPE_TECNICA){
				$equipe = true;
			}else{
				$equipe = false;
			}
		}
		
		if($equipe == false){
			echo '	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
						<tr valign="top">
							<td width="33%">
								<div>
									<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
										<tr>
											<td align="center">
												<b>PAR</b><br />
												Cursistas: '.count( $arPar ).'<br />  
											</td>
										</tr>
										<tr>
											<td>';
												$this->monta_lista( $arPar, array( "CPF", "Nome" ), 20, 10, 'N', '', '' );
			echo '							</td>
										</tr>
									</table>
								</div>
							</td>	
							<td width="33%">
								<div>
									<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
										<tr>
											<td align="center">
												<b>Monitoramento</b><br />
												Cursistas: '. count( $arMonitoramento ) .'<br />  
											</td>
										</tr>
										<tr>
											<td>';
												$this->monta_lista( $arMonitoramento, array( "CPF", "Nome", "Situação" ), 20, 10, 'N', '', '' );
			echo '							</td>
										</tr>
									</table>
								</div>
							</td>	
							<td width="34%">
								<div>
									<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
										<tr>
											<td align="center">
												<b>Programa</b><br />
												Cursistas: '. count( $arPrograma ) .'<br />  
											</td>
										</tr>
										<tr>
											<td>';
												$this->monta_lista( $arPrograma, array( "CPF", "Nome", "Situação" ), 20, 10, 'N', '', '' );
			echo '							</td>
										</tr>
									</table>
								</div>
								<div style="padding: 5px 0px 5px 15px;">
									<span id="adCursista" name="adCursista" style="cursor:pointer;display:block;float:left" onclick="return popupSelecionarCursista( \''.$this->mniano.'\', \''.$this->inuid.'\', \''.$this->ppsid.'\' );"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Editar / Inserir Cursista</span>
								</div><br />
							</td>	
						</tr>
					</table>
			';
		}else{
			echo '	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
						<tr valign="top">							
							<td width="100%">
								<div>
									<table width="60%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
										<tr>
											<td align="center">
												<b>Programa</b><br />
												Cursistas: '. count( $arPrograma ) .'<br />  
											</td>
										</tr>
										<tr>
											<td>';
												$this->monta_lista( $arPrograma, array( "CPF", "Nome", "Situação" ), 20, 10, 'N', '', '' );												
			echo '							</td>
										</tr>
										<tr>
											<td>
												<div style="padding: 5px 0px 5px 15px;">
													<span id="adCursista" name="adCursista" style="cursor:pointer;display:block;float:left" onclick="return popupSelecionarCursista( \''.$this->mniano.'\', \''.$this->inuid.'\', \''.$this->ppsid.'\' );"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Editar / Inserir Cursista</span>
												</div><br />
											</td>
										</tr>
									</table>									
								</div>								
							</td>	
						</tr>
					</table>
			';
		}
	}
									  
	public function montarFormularioEscolasPorAno( $sbaid ){
		
		/***********************************************************************************
		*									ESCOLAS PAR									   *
		***********************************************************************************/
		
		$sqlQtdPar = " 	select count( qfa.entid ) as qtdEscolas, sum( qfa.qfaqtd ) as qtdGeral 
						from cte.qtdfisicoano qfa
							inner join entidade.entidade e on e.entid = qfa.entid
						where qfa.sbaid = '$sbaid'
						and qfaano = '{$this->mniano}'";

		$arQtdPar = $this->pegaLinha( $sqlQtdPar ); 						
		$arQtdPar = $arQtdPar ? $arQtdPar : array( 'qtdEscolas' => '0', 'qtdGeral' => '0' ); 						

		$sqlPar = " select distinct e.entcodent, e.entnome, coalesce( qfa.qfaqtd, 0 )
					from cte.qtdfisicoano qfa
						inner join entidade.entidade e on e.entid = qfa.entid
					where qfa.sbaid = '$sbaid'
					and qfaano = '{$this->mniano}'
					order by e.entnome";
		
		/***********************************************************************************
		*								ESCOLAS MONITORAMENTO						   	   *
		***********************************************************************************/		

		$sqlQtdMonitoramento = "select count( ees.eesentescola ) as qtdEscolas, sum( ees.eesentqtd ) as qtdGeral
								from cte.monitoramentosubacao m
									inner join cte.execucaosubacao es on es.mntid = m.mntid
									inner join cte.entidadeexecucaosubacao ees on ees.exeid = es.exeid
									inner join entidade.entidade e on e.entid = ees.eesentescola
								where m.sbaid = '$sbaid'
								and es.exeid = (
									select coalesce(es.exeid, 0) as exeqtd
									from cte.monitoramentosubacao m
										inner join cte.execucaosubacao es on es.mntid = m.mntid
										inner join cte.periodoreferencia pr on pr.perid = es.perid
									where m.sbaid = $sbaid and pr.perano = '{$this->mniano}' order by pr.perid desc limit 1
								)";
								
		$arQtdMonitoramento = $this->pegaLinha( $sqlQtdMonitoramento ); 						
		$arQtdMonitoramento = $arQtdMonitoramento ? $arQtdMonitoramento : array( 'qtdEscolas' => '0', 'qtdGeral' => '0' ); 					
					
		$sqlMonitoramento = "	select distinct e.entcodent, e.entnome, coalesce( ees.eesentqtd, 0 )
								from cte.monitoramentosubacao m
									inner join cte.execucaosubacao es on es.mntid = m.mntid
									inner join cte.entidadeexecucaosubacao ees on ees.exeid = es.exeid
									inner join entidade.entidade e on e.entid = ees.eesentescola
								where m.sbaid = '$sbaid'
								and es.exeid = (
									select coalesce(es.exeid, 0) as exeqtd
									from cte.monitoramentosubacao m
										inner join cte.execucaosubacao es on es.mntid = m.mntid
										inner join cte.periodoreferencia pr on pr.perid = es.perid
									where m.sbaid = $sbaid and pr.perano = '{$this->mniano}' order by pr.perid desc limit 1
								)
								order by e.entnome";
		
		/***********************************************************************************
		*								ESCOLAS PROGRAMA							   	   *
		***********************************************************************************/
								
		$sqlQtdPrograma = "	select count( mie.entid ) as qtdEscolas, sum( mie.mneqtd ) as qtdGeral
							from cte.monitoramentointerno mi
								inner join cte.monitoramentointernoentidade mie on mie.mniid = mi.mniid
								inner join entidade.entidade e on e.entid = mie.entid
							where mi.inuid = '{$this->inuid}'
							and mi.ppsid = '{$this->ppsid}'
							and mi.mniano = '{$this->mniano}'";
						
		$arQtdPrograma = $this->pegaLinha( $sqlQtdPrograma ); 						
		$arQtdPrograma = $arQtdPrograma ? $arQtdPrograma : array( 'qtdEscolas' => '0', 'qtdGeral' => '0' ); 					
		
		$sqlPrograma = "select distinct e.entcodent, e.entnome, coalesce( mie.mneqtd, 0 )
						from cte.monitoramentointerno mi
							inner join cte.monitoramentointernoentidade mie on mie.mniid = mi.mniid
							inner join entidade.entidade e on e.entid = mie.entid
						where mi.inuid = '{$this->inuid}'
						and mi.ppsid = '{$this->ppsid}'
						and mi.mniano = '{$this->mniano}'
						and mi.mnidtvalidacao is not null
						order by e.entnome";
		
		$perfis = cte_arrayPerfil();
		for($x=0; $x<count($perfis); $x++){
			//172, 167, 125*, 288
			if($perfis[$x] == CTE_PERFIL_EQUIPE_TECNICA){
				$equipe = true;
			}else{
				$equipe = false;
			}
		}
		
		if($equipe == false){
			echo '	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
						<tr valign="top">
							<td width="33%">
								<div>
									<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
										<tr>
											<td align="center">
												<b>PAR</b><br />
												Escolas: '.$arQtdPar['qtdescolas'].'<br />  
												Quantidade: '.$arQtdPar['qtdgeral'].'<br />  
											</td>
										</tr>
										<tr>
											<td>';
												$this->monta_lista( $sqlPar, array( "Código INEP", "Escola", "Qtd" ), 20, 10, 'N', '', '' );
			echo '							</td>
										</tr>
									</table>
								</div>
							</td>	
							<td width="33%">
								<div>
									<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
										<tr>
											<td align="center">
												<b>Monitoramento</b><br />
												Escolas: '.$arQtdMonitoramento['qtdescolas'].'<br />  
												Quantidade: '.$arQtdMonitoramento['qtdgeral'].'<br />  
											</td>
										</tr>
										<tr>
											<td>';
												$this->monta_lista( $sqlMonitoramento, array( "Código INEP", "Escola", "Qtd" ), 20, 10, 'N', '', '' );
			echo '							</td>
										</tr>
									</table>
								</div>
							</td>	
							<td width="34%">
								<div>
									<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
										<tr>
											<td align="center">
												<b>Programa</b><br />
												Escolas: '.$arQtdPrograma['qtdescolas'].'<br />  
												Quantidade: '.$arQtdPrograma['qtdgeral'].'<br />  
											</td>
										</tr>
										<tr>
											<td>';
												$this->monta_lista( $sqlPrograma, array( "Código INEP", "Escola", "Qtd" ), 20, 10, 'N', '', '' );
			echo '							</td>
										</tr>
									</table>
								</div>
								<div style="padding: 5px 0px 5px 15px;">
									<span id="adEscolas" name="adEscolas" style="cursor:pointer;display:block;float:left" onclick="return popupSelecionarEscolas( \''.$this->mniano.'\', \''.$this->inuid.'\', \''.$this->ppsid.'\' );"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Editar / Inserir Escolas</span>
								</div><br />
							</td>	
						</tr>
					</table>
			';
		} else {
			echo '	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
						<tr valign="top">
							<td width="100%">
								<div>
									<table width="60%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
										<tr>
											<td align="center">
												<b>Programa</b><br />
												Escolas: '.$arQtdPrograma['qtdescolas'].'<br />  
												Quantidade: '.$arQtdPrograma['qtdgeral'].'<br />  
											</td>
										</tr>
										<tr>
											<td>';
												$this->monta_lista( $sqlPrograma, array( "Código INEP", "Escola", "Qtd" ), 20, 10, 'N', '', '' );
			echo '							</td>
										</tr>
										<tr>
											<td>
												<div style="padding: 5px 0px 5px 15px;">
													<span id="adEscolas" name="adEscolas" style="cursor:pointer;display:block;float:left" onclick="return popupSelecionarEscolas( \''.$this->mniano.'\', \''.$this->inuid.'\', \''.$this->ppsid.'\' );"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Editar / Inserir Escolas</span>
												</div><br />
											</td>
										</tr>
									</table>
								</div>								
							</td>	
						</tr>
					</table>
			';
		}
	}
	
	public function carregarPorInuidPpsidAno( $inuid, $ppsid, $mniano ){
		
		$this->inuid = $inuid ? $inuid : $_SESSION["inuid"];
		
		$sql = " select mniid, inuid, ppsid, mniano, mniqtd from $this->stNomeTabela WHERE inuid = $inuid and ppsid = $ppsid and mniano = $mniano ";
		$arResultado = $this->pegaLinha( $sql );
		
		$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );		
	}
	
	function validarRegistrosInseridosUsuario($usucpf, $usucpfautoriza){
		
		$sql = "UPDATE {$this->stNomeTabela} SET mnidtvalidacao = NOW(), usucpfvalida = '{$usucpfautoriza}' WHERE usucpf = '{$usucpf}' AND mnidtvalidacao IS NULL";
		$this->executar($sql);
		
	}
									  
}