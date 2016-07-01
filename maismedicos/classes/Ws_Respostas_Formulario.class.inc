<?php

/** 
 * Esta classe trata os formulário de supervisão do projeto MAis Médicos
 * via WebService originado do UNASUS. 
 * 
 * @author Wescley Lima <wescley.lima@mec.gov.br>
 * @copyright jan/2015, SIMEC. 
 * @access public  
 * @subpackage Ws_Respostas_Formulario_Itens
 * 
 * @var arrIdsFormularios
 * 
 * IDS DOS FORMULÁRIOS 
 * 
 * 33 - Primeiras impressões
 * 35 - Primeira Visita de Supervisão
 * 36 - Supervisão Prática
 * 39 - Primeiras impressões - DSEI
 * 40 - Primeira Visita de Supervisão "IN LOCO" – DSEI
 * 43 - Supervisão Prática – DSEI
 */

include_once APPRAIZ . 'maismedicos/classes/Ws_Unasus_Soap.class.inc';
	
class Ws_Respostas_Formulario extends Ws_Unasus_Soap{
	
	protected $arrIdsFormularios;
	
	protected $arrFormulariosMaisMedicos;
		
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.ws_respostas_formulario";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rfoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'rfoid' => null, 
									  	'wssdata' => null, 
									  	'idpublicarformulario' => null, 
									  	'idformulario' => null, 
									  	'dsformulario' => null, 
									  	'cpfenvio' => null, 
									  	'dtpublicacao' => null, 
									  	'dtultimaatualizacao' => null, 
									  	'dtenvio' => null, 
									  	'wssstatus' => null, 
									  	'status' => null, 
									  	'itens_processado' => null, 
									  	'itens_processado_data' => null, 
									  	'dtvisita' => null, 
									  	'cpfprofissional' => null, 
									  	'nuciclo' => null, 
									  	'uniid' => null, 
									  	'dtvisitaresposta' => null, 
									  	'muncod' => null, 
									  	'estuf' => null, 
									  	'nosupervisor' => null, 
									  	'cpfsupervisor' => null, 
									  	'muncodprofissional' => null, 
									  	'estufprofissional' => null, 
									  	'nomunicipioprofissional' => null, 
									  	'dsei' => null, 
									  	'noprofissional' => null, 
									  	'coinstituicao' => null, 
									  	'noinstituicao' => null, 
									  	'nuetapa' => null, 
									  );
    
    /**
     * Construtor
     */
    public function __construct()    
    {
    	$this->arrIdsFormularios = array(33,35,36,39,40,43);    	
    }
    
    /**
     * Puxa os formulários do UNASUS via WebService
     * 
     * @param integer $idFormulario
     * @return array
     */
    private function getIdPublicar($idFormulario)
    {
    	$conexao_ws = $this->conexaoWS();
    	$this->arrFormulariosMaisMedicos = $conexao_ws->getIdPublicar(self::TOKEN_WS,$idFormulario);
    	return $this->arrFormulariosMaisMedicos;
    }
    
    /**
     * Puxa os formulários do UNASUS via WebService
     * 
     * @param integer $idFormulario
     * @param string $stMes 
     * @param string $stAno 
     * @return array
     */
    private function getIdPublicarMaisMedicos($idFormulario, $stMes = null, $stAno = null)
    {
    	$conexao_ws = $this->conexaoWS();
    	if(!empty($stMes) && !empty($stAno)){
    		$this->arrFormulariosMaisMedicos = $conexao_ws->getIdPublicarMaisMedicos(self::TOKEN_WS,$idFormulario, $stMes, $stAno);
    	}else{
    		$this->arrFormulariosMaisMedicos = $conexao_ws->getIdPublicarMaisMedicos(self::TOKEN_WS,$idFormulario);
    	}
    	return $this->arrFormulariosMaisMedicos;
    }
    
    /**
     * Importar formulários do UNASUS, pode ser passado o mês que deseja importar
     * 
     * @param string $stMesAno ex.: 01/2015
     * @return string
     */
    public function importaFormulariosMaisMedicos($stMesAno = null)
    {
		
    	// Contador geral dos formulários importados
    	$totalImportados=0;
    	$totalModificados=0;
    	$totalNovos=0;
    	
    	$arMesAno = explode('/', $stMesAno);
    	if($arMesAno[0]>0 && $arMesAno[1]>0){
    		$txt .= "<h1>Mês de referência: {$arMesAno[0]}/{$arMesAno[1]}";
    	}else{
    		$txt .= "<h1>Todos formulários</h1>";    		
    	}
    	
    	// Recupera formularios existentes no SIMEC
    	$sql = "select distinct
		    				idpublicarformulario,
		    				dtultimaatualizacao,
		    				status
		    			from maismedicos.ws_respostas_formulario
		    			where wssstatus = 'A'";
    	 
    	$rsFormsExistentes = $this->carregar($sql);
    	
    	// Monta array para comparação com dados da UNASUS
    	if($rsFormsExistentes){
    		foreach($rsFormsExistentes as $formulario){
    			$arFormsExistentes[$formulario['idpublicarformulario']] = array( 'idpublicarformulario' => $formulario['idpublicarformulario'], 'status' => $formulario['status'], 'dtultimaatualizacao' => $formulario['dtultimaatualizacao'] );
    		}
    	}
    	$arFormsExistentes = $arFormsExistentes ? $arFormsExistentes : array();
    	
    	// Deleta array de resultado da query da memoria
    	unset($rsFormsExistentes);
    	
    	foreach($this->arrIdsFormularios as $idFormulario){
    		
    		// Inicia contadores
    		$countForms=0;
    		$countFormsNovos=0;
    		$countFormsModificados=0;
    		$countFormsNaoModificados=0;
    		
    		if(is_array($arMesAno)){
    			
    			$this->getIdPublicarMaisMedicos($idFormulario, $arMesAno[0], $arMesAno[1]);
    		}else{
    			$this->getIdPublicarMaisMedicos($idFormulario);
    		}
    		if($this->arrFormulariosMaisMedicos->item){
    			
    			foreach($this->arrFormulariosMaisMedicos->item as $form){
    				
    				//if(in_array($form->idPublicarFormulario, array(284568,285816,284583,285813,285807,284615,284568))){
    					//var_dump($form);
    					//die;
    				//}
    				
    				if($form->status!='Finalizado') continue;
    				
    				if(is_numeric($form->idPublicarFormulario)){
    					
	    				$countForms++;
	    				
	    				$status = $form->status;
	    				$cpfenvio = $form->cpfEnvio;
	    				$idPublicarFormulario = $form->idPublicarFormulario;
	    				$dtUltimaAtualizacao = $form->dtUltimaAtualizacao ? "'".$form->dtUltimaAtualizacao."'" : 'null';
	    				$dtPublicacao = $form->dtPublicacao ? "'".$form->dtPublicacao."'" : 'null';
	    				$dtEnvio = $form->dtEnvio ? "'".$form->dtEnvio."'" : 'null';
	    				$nuMes = $form->nuMes ? $form->nuMes : 'null';
	    				$nuAno = $form->nuAno ? $form->nuAno : 'null';

	    				if($arFormsExistentes[$idPublicarFormulario]['dtultimaatualizacao'] != $form->dtUltimaAtualizacao && is_numeric($arFormsExistentes[$idPublicarFormulario]['idpublicarformulario'])){
	    					
	    					// Formularios editados
	    					++$countFormsModificados;	    					
	    					
	    					$arIdsDesativar[] = $idPublicarFormulario;
	    					
	    					$arSqlInsert[] = "('{$cpfenvio}', {$idFormulario}, {$idPublicarFormulario}, {$dtUltimaAtualizacao}, {$dtPublicacao}, '{$status}', {$dtEnvio}, {$nuMes}, {$nuAno})";
							
	    				}else if(!is_numeric($arFormsExistentes[$idPublicarFormulario]['idpublicarformulario'])){
	    					
	    					// Novos formularios
	    					++$countFormsNovos;
	    					
	    					$arSqlInsert[] = "('{$cpfenvio}', {$idFormulario}, {$idPublicarFormulario}, {$dtUltimaAtualizacao}, {$dtPublicacao}, '{$status}', {$dtEnvio}, {$nuMes}, {$nuAno})";
	    					
						}else{
							
							// Formularios nao modificados
							++$countFormsNaoModificados;
							
						}
    				}
    			}
			}
			
			$comita=false;
			
	    	// Monta o update para desativar os forms alterados
			if(is_array($arIdsDesativar)){
			
				$sqlUpdate = "update maismedicos.ws_respostas_formulario set wssstatus = 'I' where idpublicarformulario in (".implode(',',$arIdsDesativar).");";
			
				$this->executar($sqlUpdate);
			}
			unset($arIdsDesativar);
			
			if($arSqlInsert){
				
				$sqlInsert = "insert into maismedicos.ws_respostas_formulario
									(cpfenvio, idformulario, idpublicarformulario, dtultimaatualizacao, dtpublicacao, status, dtenvio, numes, nuano)
								values ".implode(',', $arSqlInsert).';';
				
				$this->executar($sqlInsert);
				$comita=true;
			}
			unset($arSqlInsert);
			
			// Deleta array resultado do WebService da memoria
			unset($this->arrFormulariosMaisMedicos->item);
			
			if($comita) $this->commit();
			
			$nomeFormulario = $this->recuperaNomeIdFormulario($idFormulario);
			
			$totalImportados += ($countFormsModificados+$countFormsNovos);
			$totalModificados += $countFormsModificados;
			$totalNovos += $countFormsNovos;
			
			$txt .= "<h3>{$nomeFormulario}</h3>
					{$countForms} recebido(s)<br/>
					{$countFormsModificados} atualizado(s)<br/>
					{$countFormsNovos} novo(s)<br/>
					{$countFormsNaoModificados} sem atualização(ões)<br/>					
					";
			
		}
		
		unset($arFormsExistentes);
		
		$txt = $txt."<p><b>Total de formulários (atualizados {$totalModificados} e novos {$totalNovos}):</b> <font size='3'>{$totalImportados}</font></p>";
		
		echo $txt;
				
		$this->enviarEmailAuditoriaRotinas('Atualização dos formulários Mais Médicos', $txt);
		
		return $txt;
    				 
    }
    
    /**
     * Muda a flag para processar as respotas novamente
     */
    private function devolveFormulariosDatasVisitaVazias()
    {
    	$sql = "select distinct rfoid from (
						select
							rfoid,
							coalesce((select count(*) from maismedicos.ws_respostas_formulario_itens i where i.wssstatus = 'A' and i.rfoid = f.rfoid),0) as itens
						from maismedicos.ws_respostas_formulario f
    					where f.wssstatus = 'A'
	    				and f.status = 'Finalizado'
	    				and f.idformulario in (35,36,04,43)
	    				and f.dtvisita is null
    					and itens_processado = true
					) as foo where itens = 0;";
    	
    	$formsSemRespostas = $this->carregarColuna($sql);
    	
    	if(count($formsSemRespostas)>0){
    		$sqlSemResposta = "update maismedicos.ws_respostas_formulario set itens_processado = false where rfoid in (".implode(',',$formsSemRespostas).");";
    		$this->executar($sqlSemResposta);
    		$this->commit();
    	}
    }
    
    public function inativaFormulariosDataVisitaErrada()
    {
    	$sql = "select to_char(dtvisita, 'DD/MM/YYYY') as dtvisita, count(*) 
    			from maismedicos.ws_respostas_formulario  where wssstatus = 'A' and status = 'Finalizado' and idformulario in (35,36,40,43)
				and (to_char(dtvisita, 'YYYY-MM') < '2013-10' or to_char(dtvisita, 'YYYY-MM') > to_char(now(), 'YYYY-MM') or dtvisita is null)
    			--and cpfenvio is not null and dtenvio is not null
    			and itens_processado = true
    			group by dtvisita";
    	
    	$rsDatasErradas = $this->carregar($sql);
    	
    	if($rsDatasErradas){
    		
    		foreach($rsDatasErradas as $datas){
    			$txt .= "<p>".$datas['count']." fomulário(s) com data de visita ".($datas['dtvisita'] ? $datas['dtvisita'] : 'vazia')."</p>";
    		}
    			    	
	    	$sql = "update maismedicos.ws_respostas_formulario set wssstatus = 'I' where rfoid in (
						select rfoid from maismedicos.ws_respostas_formulario  where wssstatus = 'A' and status = 'Finalizado' and idformulario in (35,36,40,43)
	    				and itens_processado = true
	    				--and cpfenvio is not null and dtenvio is not null
	    				and itens_processado = true
						and (to_char(dtvisita, 'YYYY-MM') < '2013-10' or to_char(dtvisita, 'YYYY-MM') > to_char(now(), 'YYYY-MM') or dtvisita is null)
					);";
	    	
	    	$this->executar($sql);
	    	$this->commit();
    	}
    	
    	echo $txt;
    	
    	return $txt;
    	
    }
    
	public function inativaFormulariosCpfVazio()
    {
    	$sql = "select count(distinct idpublicarformulario) 
    			from maismedicos.ws_respostas_formulario  where wssstatus = 'A' and status = 'Finalizado' and idformulario in (35,36,40,43)
    			--and cpfenvio is not null and dtenvio is not null
    			and itens_processado = true
				and (cpfprofissional is null or cpfprofissional = '' or dsei is null)";
    	
    	$rsErro = $this->pegaUm($sql);
    	
    	if($rsErro){
    		
	    	$sql = "update maismedicos.ws_respostas_formulario set wssstatus = 'I' where rfoid in (
						select rfoid from maismedicos.ws_respostas_formulario  where wssstatus = 'A' and status = 'Finalizado' and idformulario in (35,36,40,43)
	    				and itens_processado = true 
	    				--and cpfenvio is not null and dtenvio is not null
	    				and itens_processado = true
						and (cpfprofissional is null or cpfprofissional = '' or dsei is null)
					);";
	    	
	    	$this->executar($sql);
	    	$this->commit();
	    	
    	}
    	
    	$txt = '<p>'.($rsErro ? $rsErro : 0)." formulário(s) com cpf de envio vazio</p>";
    	
    	echo $txt;
    	
    	return $txt;
    }
    
    public function atualizaCodigoUniversidadeFormularios()
    {
    	$sql = "select count(distinct idpublicarformulario)
				from maismedicos.ws_respostas_formulario f
				left join maismedicos.universidade  u 
				on trim(lower(removeacento(u.uninome))) = trim(lower(removeacento(replace(replace(replace(f.noinstituicao,' (SUSAM)',''),' - RECIFE',''),' (DIAMANTINA)',''))))
				where wssstatus = 'A' and status = 'Finalizado' and idformulario in (35,36,40,43)
				and f.uniid is null
    			and itens_processado = true
				and f.noinstituicao is not null";
    	
    	$rs1 = $this->pegaUm($sql);
    	
    	if($rs1){
    	
	    	$sql = "update maismedicos.ws_respostas_formulario a set uniid = b.uniid
					from (
					select u.uniid, f.uniid as uniidform, u.uninome as nomeuni, f.noinstituicao as nomeform, f.rfoid
					from maismedicos.ws_respostas_formulario f
					left join maismedicos.universidade  u 
					on trim(lower(removeacento(u.uninome))) = trim(lower(removeacento(replace(replace(replace(f.noinstituicao,' (SUSAM)',''),' - RECIFE',''),' (DIAMANTINA)',''))))
					where wssstatus = 'A' and status = 'Finalizado' and idformulario in (35,36,40,43)
					and f.uniid is null
	    			and itens_processado = true
					and f.noinstituicao is not null
					) as b where b.rfoid = a.rfoid;";
	    	
	    	$this->executar($sql);
	    	$this->commit();
	    	
	    	$sql = "select count(distinct idpublicarformulario)
					from maismedicos.ws_respostas_formulario f
					left join maismedicos.universidade  u
					on trim(lower(removeacento(u.uninome))) = trim(lower(removeacento(replace(replace(replace(f.noinstituicao,' (SUSAM)',''),' - RECIFE',''),' (DIAMANTINA)',''))))
					where wssstatus = 'A' and status = 'Finalizado' and idformulario in (35,36,40,43)
					and f.uniid is null
	    			and itens_processado = true
					and f.noinstituicao is not null";
	    	 
	    	$rs2 = $this->pegaUm($sql);
	    	
    	}
    	
    	$txt = '<p>'.($rs1 ? $rs1-$rs2 : 0)." formulário(s) sem código da instituição</p>";
    	
		echo $txt;
    	
    	return $txt;
    }
    
    public function inativaFormulariosMuncodProfissionalVazio()
    {
    	$sql = "select distinct
						count(distinct idpublicarformulario)
					from maismedicos.ws_respostas_formulario f
					where f.wssstatus = 'A'
					and f.status = 'Finalizado'
					and f.itens_processado = true
    				and f.cpfprofissional is not null
					and f.idformulario in ( 35, 36, 40, 43 )
					and f.muncodprofissional is null";
    	 
    	$rsErro = $this->pegaUm($sql);
    	
    	if($rsErro){
    		if($rsErro){
    			$sql = "UPDATE maismedicos.ws_respostas_formulario con SET wssstatus = 'I' 
    					WHERE wssstatus = 'A' and status= 'Finalizado' and idformulario in (35,36,40,43)
    					and muncodprofissional is null and cpfprofissional is not null;";
    		
    			$this->executar($sql);
    			$this->commit();
    		}
    	}
    	
    	$txt = "<p>".($rsErro ? $rsErro : 0)." formulário(s) com código do IBGE do médico vazio</p>";
    	 
    	echo $txt;
    	 
    	return $txt;
    }
    
    public function atualizaNumeroCicloFormularios()
    {
    	
    	$sql = "select distinct
						f.rfoid,						
						m.nuciclo,						
						m.nuetapa
					from maismedicos.ws_respostas_formulario f
					join maismedicos.ws_profissionais m on m.nucpf = f.cpfprofissional
    					and m.wssstatus = 'A'
    					and m.wssdata = (select max(wssdata) from maismedicos.ws_profissionais where wssstatus = 'A') 
    					and m.nuciclo > 0
					where f.wssstatus = 'A'
					and f.status = 'Finalizado'
					and f.itens_processado = true
    				and f.cpfprofissional is not null
					and f.idformulario in ( 35, 36, 40, 43 )
					and f.nuciclo is null and m.nuciclo is not null";
    	
    	$rsErro = $this->carregar($sql);
    	
    	if($rsErro){
	    	$sql = "--Atualiza ciclo e etapa dos medicos
					UPDATE maismedicos.ws_respostas_formulario con SET
						nuciclo 		= cob.nuciclo,					
						nuetapa			= cob.nuetapa
					FROM (
						select distinct
							f.rfoid,						
							m.nuciclo,						
							m.nuetapa
						from maismedicos.ws_respostas_formulario f
						join maismedicos.ws_profissionais m on m.nucpf = f.cpfprofissional
	    					and m.wssstatus = 'A'
	    					and m.wssdata = (select max(wssdata) from maismedicos.ws_profissionais where wssstatus = 'A') 
	    					and m.nuciclo > 0
						where f.wssstatus = 'A'
						and f.status = 'Finalizado'
	    				and f.itens_processado = true
	    				and f.cpfprofissional is not null
						and f.idformulario in ( 35, 36, 40, 43 )
						and f.nuciclo is null and m.nuciclo is not null
					) as cob
					WHERE con.rfoid = cob.rfoid;";
	    	
	    	$this->executar($sql);
	    	$this->commit();
    	}
    	
    	$txt = "<p>".($rsErro ? count($rsErro) : 0)." formulário(s) com número de ciclo vazio</p>";
    	
    	echo $txt;
    	
    	return $txt;
    }
    
    /**
     * Atualiza os dados na tabela de formulario pegando os dados da tabela de respotas
     * Serve para ficar mais leve as consultas
     * Já esta sendo feito quando puxa as respotas mais caso haja formularios com dados em brancos 
     * verifica nas respotas e atualiza a tabela de formularios
     * 
     * @return string
     */
    public function atualizaRespostasComFormulario()
    {
    	 
    	$sql = "--Atualiza ciclo, dsei e etapa dos medicos
				UPDATE maismedicos.ws_respostas_formulario con SET
					nuciclo 		= cob.nuciclo,
					dsei			= cob.dsei,
					nuetapa			= cob.nuetapa
				FROM (
					select distinct
						f.rfoid,
						f.idpublicarformulario,
						m.nuciclo,
						m.dsei,
						m.nuetapa
					from maismedicos.ws_respostas_formulario f
					join maismedicos.medico m on m.mdccpf = f.cpfprofissional
					where f.wssstatus = 'A'
					and f.status = 'Finalizado'
					and f.idformulario in ( 35, 36, 40, 43 )
					and (f.nuciclo is null or f.dsei is null or f.nuetapa is null)
				) as cob
				WHERE con.rfoid = cob.rfoid;
    
    			--Atualiza cpfenvio nulo
				UPDATE maismedicos.ws_respostas_formulario con SET
					cpfenvio	= cob.cpfsupervisor
				FROM (
					select distinct
						f.rfoid,
						f.idpublicarformulario,
						c.dsresposta as cpfsupervisor
					from maismedicos.ws_respostas_formulario f
					join maismedicos.ws_respostas_formulario_itens c on f.rfoid = c.rfoid and c.wssstatus = 'A' and c.dspergunta ilike 'CPF%' and c.dsgrupo ilike 'DADOS PESSOAIS DO SUPERVISOR%'
					where f.wssstatus = 'A'
					and f.status = 'Finalizado'
					and f.idformulario in ( 35, 36, 40, 43 )
					and f.cpfenvio is null
				) as cob
				WHERE con.rfoid = cob.rfoid;
    
    			--Atualiza muncod do profissional nao dsei
				UPDATE maismedicos.ws_respostas_formulario con SET
					muncodprofissional	= cob.muncodprofissional
				FROM (
					select
						f.rfoid,
						f.idpublicarformulario,
						m.muncod as muncodprofissional
					from maismedicos.ws_respostas_formulario f
					join territorios.municipio m on replace(lower(removeacento(m.mundescricao)),'''','') = replace(lower(removeacento(f.nomunicipioprofissional)),'''','') and lower(m.estuf) = lower(f.estufprofissional)
					where wssstatus = 'A'
					and f.status = 'Finalizado'
    				and f.idformulario in (33,35,36)
					and (f.muncodprofissional is null)
				) as cob
				WHERE con.rfoid = cob.rfoid;
    
    			--Atualiza muncod do profissional dsei
    			UPDATE maismedicos.ws_respostas_formulario con SET
					muncodprofissional	= cob.muncod,
					estufprofissional	= cob.estuf
				FROM (
					select
						f.rfoid,
						f.cpfprofissional,
						f.muncodprofissional,
						d.muncod,
						d.estuf
					from maismedicos.ws_respostas_formulario f
					join maismedicos.medico d on d.mdccpf = f.cpfprofissional
					where f.wssstatus = 'A'
					and f.status = 'Finalizado'
					and f.idformulario in (39,40,43)
					and (f.muncodprofissional is null or f.estufprofissional is null)
				) as cob
				WHERE con.rfoid = cob.rfoid;
    
    			--Atualiza data visita dsei
    			UPDATE maismedicos.ws_respostas_formulario con SET
					dtvisita 	 = cob.dtvisita,
					dtvisitaresposta = dtvisitatmp
				FROM (
					select distinct
						f.rfoid,
						f.idpublicarformulario,
						d.dsresposta::date as dtvisita,
						d.dsresposta as dtvisitatmp
					from maismedicos.ws_respostas_formulario f
					join maismedicos.ws_respostas_formulario_itens d on f.rfoid = d.rfoid and d.wssstatus = 'A' and d.dspergunta ilike 'Informe a data da Supervisao Locorregional%'
					where f.wssstatus = 'A'
					and f.status = 'Finalizado'
					and f.idformulario in ( 43 )
					and f.dtvisita is null
				) as cob
				WHERE con.rfoid = cob.rfoid;
    
    			--Correcao na mao dos municipios nao encontrados
    			update maismedicos.ws_respostas_formulario set muncodprofissional = '1100130' where wssstatus = 'A' and status = 'Finalizado' and nomunicipioprofissional = 'MACHADINHO DOESTE' and estufprofissional = 'RO' and muncodprofissional is null;
				update maismedicos.ws_respostas_formulario set muncodprofissional = '1720499' where wssstatus = 'A' and status = 'Finalizado' and nomunicipioprofissional = 'SAO VALERIO DA NATIVIDADE' and estufprofissional = 'TO' and muncodprofissional is null;
				update maismedicos.ws_respostas_formulario set muncodprofissional = '1505551' where wssstatus = 'A' and status = 'Finalizado' and nomunicipioprofissional = 'PAU DARCO' and estufprofissional = 'PA' and muncodprofissional is null;
				update maismedicos.ws_respostas_formulario set muncodprofissional = '3305901' where wssstatus = 'A' and status = 'Finalizado' and nomunicipioprofissional = 'TRAJANO DE MORAIS' and estufprofissional = 'RJ' and muncodprofissional is null;
				update maismedicos.ws_respostas_formulario set muncodprofissional = '3303807' where wssstatus = 'A' and status = 'Finalizado' and nomunicipioprofissional = 'PARATI' and estufprofissional = 'RJ' and muncodprofissional is null;
				update maismedicos.ws_respostas_formulario set muncodprofissional = '2601607' where wssstatus = 'A' and status = 'Finalizado' and nomunicipioprofissional = 'BELEM DE SAO FRANCISCO' and estufprofissional = 'PE' and muncodprofissional is null;
    			";
    	 
    	$this->executar($sql);
    	$this->commit();
    	 
    	$txt = '<p>Dados atualizados</p>';
    	echo $txt;
    	 
    	return $txt;
    }
    
    public function recuperaNomeIdFormulario($idFormulario)
    {
    	switch ($idFormulario){
    		case 33:
    			$nome = 'Primeiras impressões';
    			break;
    		case 35:
    			$nome = 'Primeira Visita de Supervisão';
    			break;    			
    		case 36:
    			$nome = 'Supervisão Prática';
    			break;
    		case 39:
    			$nome = 'Primeiras impressões - DSEI';
    			break;
    		case 40: 
    			$nome = 'Primeira Visita de Supervisão "IN LOCO" – DSEI';
    			break;
    		case 43:
    			$nome = 'Supervisão Prática – DSEI';
    			break;
    		default:
    			$nome = 'Não encontrado';
    			break;
    	}
    	
    	return $nome;
    }
    
    public function escapeStringWs($text)
    {
    	$text = pg_escape_string(trim(str_replace(array("'", "\\",'"'),array('',"/",''),$text)));
    	 
    	return $text;
    }
    
    public function recuperaPeriodosSemVisitaPorInstituicao()
    {
    	$sql = "select  
					count(distinct mdccpf) as medicos,
					uniid,uninome,periodo, unisigla
				from (
					select		
						mdc.mdccpf,	
    				uni.uniid,
						uni.uninome,
    					uni.unisigla,		
						case when to_char(now() - dtvisita, 'DD')::integer <= 30 then 'menos30'
							 when to_char(now() - dtvisita, 'DD')::integer >= 31 and to_char(now() - dtvisita, 'DD')::integer <= 60 then 'entre30e60'  
							 when to_char(now() - dtvisita, 'DD')::integer >= 61 and to_char(now() - dtvisita, 'DD')::integer <= 90 then 'entre60e90'				 
							 when to_char(now() - dtvisita, 'DD')::integer > 90 then 'mais90'
						end as periodo
					from maismedicos.medico mdc
					join maismedicos.universidade uni on uni.idunasus = mdc.coinstituicaosupervisora
						and uni.unistatus = 'A'
					left join (select 
							cpfprofissional,
							max(dtvisita) as dtvisita
						from maismedicos.ws_respostas_formulario 
						where wssstatus = 'A' and status = 'Finalizado' and idformulario in (35,36,40,43) 
						group by cpfprofissional) fom  on fom.cpfprofissional = mdc.mdccpf
						
					where mdc.mdcstatus = 'A'	
				) as foo
				group by uniid, uninome,unisigla, periodo
				order by uninome";
    	
    	return $this->carregar($sql);
    }
    
}