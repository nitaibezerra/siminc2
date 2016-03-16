<?php

include_once APPRAIZ . 'maismedicos/classes/Ws_Respostas_Formulario.class.inc';
	
class Ws_Respostas_Formulario_Itens extends Ws_Respostas_Formulario{
	
	protected $arrIdsFormularios;
	
	protected $arrRespostasFormulario;
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.ws_respostas_formulario_itens";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rfiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'rfiid' => null, 
									  	'wssdata' => null, 
									  	'rfoid' => null, 
									  	'dsdimensao' => null, 
									  	'dsgrupo' => null, 
									  	'dspergunta' => null, 
									  	'dsresposta' => null, 
									  	'wssstatus' => null, 
									  	'idpublicarformulario' => null, 
									  );
    
    /**
     * Método que busca todas as respostas dos formulários.
     * 
     * @param integer $idPublicaFormulario
     */
    private function getRespostasFormularioWS($idPublicaFormulario)
    {
    	$conexao_ws = $this->conexaoWS();
    	$this->arrRespostasFormulario = $conexao_ws->getRespostasFormulario(self::TOKEN_WS,$idPublicaFormulario);
    	return $this->arrRespostasFormulario;
    }
    
    /**
     * Método que busca somente as respostas que são interessantes para o projeto Mais Médicos
     * Ao invés de buscar todas as respostas virão somente as que serão definidas pela equipe da DDES
     *  
     * @param integer $idPublicaFormulario
     */
    protected function getRespostasFormularioMaisMedicosWS($idPublicaFormulario)
    {
    	$conexao_ws = $this->conexaoWS();
    	$this->arrRespostasFormulario = $conexao_ws->getRespostasFormularioMaisMedicos(self::TOKEN_WS,$idPublicaFormulario);
    	return $this->arrRespostasFormulario;
    }
    
    
    public function getRespostasFormularioMaisMedicos($idPublicaFormulario)
    {
    	$conexao_ws = $this->conexaoWS();
    	$this->arrRespostasFormulario = $conexao_ws->getRespostasFormularioMaisMedicos(self::TOKEN_WS,$idPublicaFormulario);
    	return $this->arrRespostasFormulario;
    }
    
    public function getRespostasFormularioMaisMedicosTodas($idPublicaFormulario)
    {
    	$conexao_ws = $this->conexaoWS();
    	$this->arrRespostasFormulario = $conexao_ws->getRespostasFormulario(self::TOKEN_WS,$idPublicaFormulario);
    	return $this->arrRespostasFormulario;
    }
    
    /**
     * Importa as respotas dos formulários
     * 
     * @return string
     */
    public function importaRespostasFormularioMaisMedicos()
    {
    	 
    	$sqlIds = "select
    				rfoid,
    				idpublicarformulario,
    				status
    			from maismedicos.ws_respostas_formulario
    			where wssstatus = 'A'
    			and status = 'Finalizado'
    			and idpublicarformulario is not null
    			and itens_processado = false";
    	 
    	// Recupera todos os forms que estao ativos e nao foram processado os itens
    	$rsForms = $this->carregar($sqlIds);
    	
//     	$rsForms = array(array('idpublicarformulario'=>292517));
    	 
    	// Inicia concatenacao do scrip update
    	$sqlUpdate = '';
    	 
    	if($rsForms){
    
    		// Inicia contadores
    		$countItens=0;
    		$countForms=0;
    		$countBlocosForms=0;
    		$countComRespostas=0;
    		$countSemRespostas=0;
    		$totalForms = count($rsForms);
    
    		// Pecorre os formularios nao processados
    		foreach($rsForms as $i => $form){
    			
    			$countForms++;
    			 
    			if($form['idpublicarformulario'])    			 
    				$this->getRespostasFormularioMaisMedicosWS($form['idpublicarformulario']);
    
    			if($this->arrRespostasFormulario->item->itemFormulario->item){
    				
    				$countComRespostas++;
    
    				$idRForm 				= $form['rfoid'] ? $form['rfoid'] : 'null';
    				$idpublicarformulario 	= $form['idpublicarformulario'] ? $this->escapeStringWs($form['idpublicarformulario']) : 'null';
    				$status 				= $form['status'] ? "'".$form['status']."'" : 'null';
    				$dsformulario 			= $this->arrRespostasFormulario->item->dsFormulario ? "'".$this->escapeStringWs($this->arrRespostasFormulario->item->dsFormulario)."'" : 'null';
    				$cpfenvio 				= $this->arrRespostasFormulario->item->cpfEnvio ? "'".$this->escapeStringWs($this->arrRespostasFormulario->item->cpfEnvio)."'" : 'null';
    				$dtenvio 				= $this->arrRespostasFormulario->item->dtEnvio ? "'".$this->escapeStringWs($this->arrRespostasFormulario->item->dtEnvio)."'" : 'null';

    				$uniid					= 'null';
    				$nuciclo				= 'null';
    				$dseiMedico				= 'null';
    				$cpfMedico 				= 'null';
    				$nomeMedico 			= 'null';
    				$nomeMunicipioMedico	= 'null';
    				$ufMunicipioMedico		= 'null';
    				$cpfSupervisor 			= 'null';
    				$nomeSupervisor 		= 'null';
    				$dataVisita 			= 'null';
    				$dataVisitaResposta 	= 'null';
    				$cpfMedicoResposta 		= 'null';
    				$nomeInstituicao		= 'null';
    				$muncodMunicipioMedico 	= 'null';
    				$codigoInstituicao		= 'null';
    					
    				foreach($this->arrRespostasFormulario->item->itemFormulario->item as $k => $item){
    
    					$countItens++;
    					//
    					$dsDimensao				= $item->dsDimensao ? "'".$this->escapeStringWs($item->dsDimensao)."'" : 'null';
						$dsGrupo 				= $item->dsGrupo ? "'".$this->escapeStringWs($item->dsGrupo)."'" : 'null';
						$dsPergunta 			= $item->dsPergunta ? "'".$this->escapeStringWs($item->dsPergunta)."'" : 'null';
						$dsResposta 			= $item->dsResposta ? "'".$this->escapeStringWs($item->dsResposta)."'" : 'null';
    
						$arInsert[] = "({$dsDimensao}, {$dsGrupo}, {$dsPergunta}, {$dsResposta}, {$idRForm}, {$idpublicarformulario})";
    
						$item->dsPergunta = str_replace(':', '', $item->dsPergunta);
    
						if(strtolower($item->dsPergunta) == strtolower('CPF') && strpos( strtolower($item->dsGrupo), strtolower('DADOS PESSOAIS DO MEDICO SUPERVISIONADO') ) !== false){

							$cpfMedicoTmp = str_replace(array('.','-'), '', $item->dsResposta);
							$rsMedico = $this->pegaLinha("select nuciclo,dsei from maismedicos.medico 
															where mdccpf = '{$cpfMedicoTmp}' and mdcstatus = 'A' limit 1");
							
							if(!$rsMedico){
								$rsMedico = $this->pegaLinha("select distinct 
																	case when decicloentrada ilike '%Primeiro ciclo%' then 1
																	     when decicloentrada ilike '%Segundo ciclo%' then 2
																	     when decicloentrada ilike '%Terceiro ciclo%' then 3
																	     when decicloentrada ilike '%Quarto ciclo%' then 4
																	     when decicloentrada ilike '%Quinto ciclo%' then 5
																	     when decicloentrada ilike '%Sexto ciclo%' then 6	     
																		else null end as nuciclo,
																	case when dsei = 'Sim' then 't' 
																		 when dsei = 'SIM' then 't' 
																		else 'f' end as dsei 
																from maismedicos.ws_profissionais  p
																where wssstatus = 'A' and nucpf = '{$cpfMedicoTmp}' 
																and p.wssdata = (select max(wssdata) from maismedicos.ws_profissionais where wssstatus = 'A') limit 1");
							}
							// CPF do Medico
							$nuciclo = $rsMedico['nuciclo'] ? $rsMedico['nuciclo'] : 'null';
							$dseiMedico = $rsMedico['dsei'] ? "'".$rsMedico['dsei']."'" : 'null';
							$cpfMedico = "'".$cpfMedicoTmp."'";
							
							//if($cpfMedicoTmp == '') ver($cpfMedico, $dseiMedico, $nuciclo, d);
    										
						}else
							if(strtolower($item->dsPergunta) == strtolower('NOME') && strpos( strtolower($item->dsGrupo), strtolower('DADOS PESSOAIS DO MEDICO SUPERVISIONADO') ) !== false){
    						
							// Nome do Medico
							$nomeMedico = "'".str_replace(array("'"), '', $item->dsResposta)."'";
    						
						}else
							if( strtolower($item->dsPergunta) == strtolower('MUNICIPIO/UF') && strpos( strtolower($item->dsGrupo), strtolower('LOCAL DE ATUACAO DO MEDICO SUPERVISIONADO') ) !== false){
    						
							// Local de atuacao do medico, muncod, mundescricao e estuf
							$municipiouf = trim($item->dsResposta);
							$localUf = substr($municipiouf, strlen($municipiouf)-2, 2);
							$localMunicipio = str_replace(array("'"), '',substr($municipiouf, 0, strlen($municipiouf)-3));
    						
							$nomeMunicipioMedico = "'".$localMunicipio."'";
							$ufMunicipioMedico = "'".$localUf."'";
//     						ver($localMunicipio, $item->dsResposta, d);
							$muncod = $this->pegaUm("(select muncod from territorios.municipio 
														where lower(removeacento(replace(replace(mundescricao,'''', ''),' D ',' D'))) = lower(removeacento('{$localMunicipio}'))
														and estuf = '{$localUf}' limit 1)");
							
							$muncod = $muncod ? $muncod : $this->municipiosProblema($localMunicipio);
							
							// Não DSEI
							$muncodMunicipioMedico = $muncod ? $muncod : 'null';
							
						}else if( strtolower($item->dsPergunta) == strtolower('Localizacao do Distrito Sanitario Especial Indigena (DSEI)/UF') && 
								strpos( strtolower($item->dsGrupo), strtolower('LOCAL DE ATUACAO DO MEDICO SUPERVISIONADO') ) !== false){
							
							if($cpfMedico){
								$rs = $this->pegaLinha("select muncod,estuf from maismedicos.medico where mdccpf = {$cpfMedico} order by mdcid desc limit 1");
								if(!$rs){
									$rs = $this->pegaLinha("select distinct m.muncod, m.estuf from maismedicos.ws_profissionais  p
														join territorios.municipio m on m.muncod = p.muncod
														where wssstatus = 'A' and nucpf = {$cpfMedico} 
														and p.wssdata = (select max(wssdata) from maismedicos.ws_profissionais where wssstatus = 'A')  limit 1");
								}
							}
							
							// Local de atuacao do medico, muncod, mundescricao e estuf
							$municipiouf = str_replace(array("'"), '',trim($item->dsResposta));
							$nomeMunicipioMedico = "'".$municipiouf."'";
							$ufMunicipioMedico = "'".$rs['estuf']."'";
							
							// DSEI
							$muncodMunicipioMedico = $rs['muncod'] ? $rs['muncod'] : 'null';
						}else
							if(strtolower($item->dsPergunta) == strtolower('CPF') && strpos( strtolower($item->dsGrupo), strtolower('DADOS PESSOAIS DO SUPERVISOR') ) !== false){
				
							// CPF do Supervisor
							$cpfSupervisor = "'".$item->dsResposta."'";
    							
						}else
							if(strtolower($item->dsPergunta) == strtolower('NOME') && strpos( strtolower($item->dsGrupo), strtolower('DADOS PESSOAIS DO SUPERVISOR') ) !== false){
				
							// Nome do Supervisor
							$nomeSupervisor = "'".str_replace(array("'"), '', $item->dsResposta)."'";
    								
						}else
							if(strtolower($item->dsPergunta) == strtolower('NOME DA INSTITUICAO SUPERVISORA') && strpos( strtolower($item->dsGrupo), strtolower('DADOS PROFISSIONAIS DO SUPERVISOR') ) !== false){
				
							// Instituicao Supervisora
							$nomeInstituicao = "'".str_replace(array("'"), ' ', $item->dsResposta)."'";
							//on trim(lower(removeacento(u.uninome))) = trim(lower(removeacento(replace(replace(replace(f.noinstituicao,' (SUSAM)',''),' - RECIFE',''),' (DIAMANTINA)',''))))
							
							$rsInstituicao = $this->pegaLinha("( select idunasus, uniid from maismedicos.universidade where unistatus = 'A' 
							and trim(lower(removeacento(uninome))) = trim(lower(removeacento(replace(replace(replace($nomeInstituicao,' (SUSAM)',''),' - RECIFE',''),' (DIAMANTINA)',''))))
							limit 1)");
							
							$uniid = $rsInstituicao['uniid'] ? $rsInstituicao['uniid'] : 'null';
							$codigoInstituicao = $rsInstituicao['idunasus'] ? $rsInstituicao['idunasus'] : 'null';
    								
						}else
							if(strtolower($item->dsPergunta) == strtolower('Data da visita') || 
									strtolower($item->dsPergunta) == strtolower('Informe a data da Supervisao Locorregional') || 
							strtolower($item->dsPergunta) == strtolower('Data inicial')){
    										
							// Data da visita
							if( strpos( strtolower($item->dsResposta), strtolower('/') ) ){
								$data = explode("/",$item->dsResposta);
								$d = $data[0];
								$m = $data[1];
								$y = $data[2];
							}else{
								$data = explode("-",$item->dsResposta);
								$d = $data[2];
								$m = $data[1];
								$y = $data[0];
							}
    													
							if(is_numeric($m) && is_numeric($d) && is_numeric($y)){
								$verificaData = checkdate($m,$d,$y);
								if ($verificaData == 1){
										$dataVisita = "'".$item->dsResposta."'";
								}
							}
    
							$dataVisitaResposta = "'".substr($item->dsResposta,0,25)."'";
						}
					}
    			
					$sqlUpdate .= "update maismedicos.ws_respostas_formulario set
							    		idpublicarformulario 	= {$idpublicarformulario},
							    		status 					= {$status},
							    		dsformulario 			= {$dsformulario},
							    		itens_processado		= true,
							    		itens_processado_data	= now(),
							    		dtvisita				= {$dataVisita},
							    		dtvisitaresposta		= {$dataVisitaResposta},
							    		noprofissional 			= {$nomeMedico},
							    		nomunicipioprofissional = {$nomeMunicipioMedico},
							    		estufprofissional 		= {$ufMunicipioMedico},
							    		cpfprofissional			= {$cpfMedico},
							    		dsei					= {$dseiMedico},
							    		nosupervisor			= {$nomeSupervisor},
							    		cpfsupervisor			= {$cpfSupervisor},
							    		noinstituicao			= {$nomeInstituicao},
							    		muncodprofissional 		= {$muncodMunicipioMedico},
							    		uniid					= {$uniid},
							    		coinstituicao 			= {$codigoInstituicao},
							    		nuciclo					= {$nuciclo}
						    		where rfoid = {$idRForm};";
    			
    			}else{
    				$countSemRespostas++;
    				$arIdsSemResposta[] = $form['idpublicarformulario'];
    			}
    
    			$countBlocosForms++;
    
    			if( $arInsert && ($countBlocosForms==100 || $countForms==$totalForms) ){
//     		ver($sqlUpdate, d);
    				$sqlInsert = "insert into maismedicos.ws_respostas_formulario_itens
    					(dsdimensao, dsgrupo, dspergunta, dsresposta, rfoid, idpublicarformulario)
    				values
    					".implode(',', $arInsert).";";

    				$this->executar($sqlInsert);
					$this->executar($sqlUpdate);
					$this->commit();
			
					unset($sqlInsert);
					unset($sqlUpdate);
					$countBlocosForms=0;
				}
    		}
    	}
    	
    	$txt = '<p>'.$countItens.' respostas para '.$countComRespostas.' formulário(s)</p>
    			 <p>'.$countSemRespostas.' formulário(s) sem respostas ('.implode(',', $arIdsSemResposta).')</p>';
    	
    	echo $txt;
    	
    	$txt = "Atualização das respostas dos formulários Mais Médicos.<br/><br/>".$txt;
    	
    	$this->enviarEmailAuditoriaRotinas('Atualização das respostas dos formulários Mais Médicos', $txt);
    	
    	return $txt;
    	
    	
    }
    
    public function recuperarRespostasPorIdUnasus($id)
    {
    	$sql = "select * from {$this->stNomeTabela} where idpublicarformulario = {$id} and wssstatus = 'A'";
    	return $this->carregar($sql);
    }
    
    public function municipiosProblema($dscMunicipio=null)
    {
    	
    	switch ($dscMunicipio){
    		case 'BELEM DE SAO FRANCISCO':
    			//Belém do São Francisco
    			$muncod = '2601607';
    			break;
    		case 'PARATI':
    			//Paraty
    			$muncod = '3303807';
    			break;
    		case 'TRAJANO DE MORAIS':
    			//Trajano de Moraes
    			$muncod = '3305901';
    			break;
    		case 'EMBU':
    			//Embu das Artes
    			$muncod = '3515004';
    			break;
    		case 'SAO VALERIO DA NATIVIDADE':
    			//Natividade
    			$muncod = '1714203';
    			break;
    		default:
    			$muncod=false;
    			break;
    	}
    	
    	return $muncod;

    }
}