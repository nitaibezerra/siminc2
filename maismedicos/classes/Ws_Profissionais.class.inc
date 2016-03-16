<?php
	
include_once APPRAIZ . 'maismedicos/classes/Ws_Unasus_Soap.class.inc';

class Ws_Profissionais extends Ws_Unasus_Soap{
	
	protected $arrProfissionais;
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.ws_profissionais";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "wssid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'wssid' => null, 
									  	'wssdata' => null, 
									  	'noProfissional' => null, 
									  	'nuCpf' => null, 
									  	'deEmail' => null, 
									  	'deFone' => null, 
									  	'delogradouro' => null, 
									  	'debairro' => null, 
									  	'decomplemento' => null, 
									  	'nucep' => null, 
									  	'municprofissional' => null, 
									  	'ufprofissional' => null, 
									  	'ibgeprofissional' => null, 
									  	'dstitulacao' => null, 
									  	'noformacaoprofissional' => null, 
									  	'dscomplementoformacao' => null, 
									  	'idinstituicao' => null, 
									  	'nuCnpjInstituicao' => null, 
									  	'noinstituicao' => null, 
									  	'municinstituicao' => null, 
									  	'ufInstituicao' => null, 
									  	'nomae' => null, 
									  	'dtNasc' => null, 
									  	'bancocomum_agenciaconta' => null, 
									  	'bancocomum_dvagenciaconta' => null, 
									  	'bancocomum_contacorrente' => null, 
									  	'bancocomum_dvcontacorrente' => null, 
									  	'bancocomum_bancoconta' => null, 
									  	'bancocomum_municipioconta' => null, 
									  	'bancocomum_ufconta' => null, 
									  	'bancobolsa_agenciacontabolsa' => null, 
									  	'bancobolsa_dvagenciacontabolsa' => null, 
									  	'bancobolsa_bancocontabolsa' => null, 
									  	'bancobolsa_municipiocontabolsa' => null, 
									  	'bancobolsa_ufcontabolsa' => null, 
									  	'tutorresponsavel' => null, 
									  	'situacao' => null, 
									  	'validado' => null, 
									  	'tpSexo' => null, 
									  	'nuRg' => null, 
									  	'noIesGraduacao' => null, 
									  	'nuAnoGraduacao' => null, 
									  	'sgConselhoProfissional' => null, 
									  	'noConselhoProfissional' => null, 
									  	'nuConselho' => null, 
									  	'ufConselho' => null, 
									  	'noPaisOrigem' => null, 
									  	'flIntercambio' => null, 
									  	'deCicloEntrada' => null, 
									  	'municipioNascimento' => null, 
									  	'instituicaoSupervisora' => null, 
									  	'municipioInstituicao' => null, 
									  	'municipioAtuacao' => null, 
									  	'wssstatus' => null,
									  	'uniid' => null,
									  	'muncod' => null, 
									  	'ibgeAtuacao' => null,
									  	'nuciclo' => null,
									  	'nuetapa' => null,
									  	'dsei' => null,
									  	'coInstituicaoSupervisora' => null
									  );
    
    private function getProfissionaisWS()
    {
    	$conexao_ws = $this->conexaoWS();
    	$this->arrProfissionais = $conexao_ws->getProfissionais(self::TOKEN_WS,4);
    	return $this->arrProfissionais;
    }
    
    public function importaProfissionaisMedicosMaisMedicos()
    {
    	$this->importaProfissionaisMaisMedicosWS();
    	 
    	// ATUALIZA TODOS OS MEDICOS ATIVOS
    	$sql = "delete from maismedicos.medico;
    
    			insert into maismedicos.medico ( mdcnome, mdcfasealocacao, muncod, mdccpf, mdcemail, mdcfone, mdcnasc, mdcsexo, uniid, mdcstatus, nuciclo, estuf, mdcnomeinstituicao, nuetapa, dsei, coinstituicaosupervisora )
				select 
					noprofissional, '', m.muncod, a.nucpf, deemail, defone, dtnasc, tpsexo, u.uniid, case when a.situacao = 1 then 'A' else 'I' end as wssstatus, a.nuciclo, m.estuf, instituicaosupervisora, a.nuetapa,
					case when a.dsei = 'Sim' then true else false end as dsei, a.coinstituicaosupervisora
				from maismedicos.ws_profissionais a
				inner join ( select max(wssid) as wssid, nucpf from maismedicos.ws_profissionais where wssstatus = 'A' group by nucpf ) b ON b.wssid = a.wssid
				inner join territorios.municipio m ON substr(m.muncod, 1, 6) = a.ibgeatuacao
				left join maismedicos.universidade u on u.idunasus = a.coinstituicaosupervisora
				where wssstatus = 'A' and unistatus = 'A' and situacao = 1 and date(wssdata) = date((select max(wssdata) from maismedicos.ws_profissionais));
				    			";
    
    	$this->executar($sql);
    	$this->commit();
    
    	$txt = '<p>'.$this->pegaUm("select count(*) from maismedicos.medico where mdcstatus = 'A'")." profissionais atualizados</p>";
    	
    	echo $txt;
    	
    	$txt = "Atualização do Cadastro de Profissionais do Mais Médicos pelo WS.<br/></br>".$txt;
    	
    	$this->enviarEmailAuditoriaRotinas('Atualização do Cadastro de Profissionais do Mais Médicos', $txt);
    	
    	return $txt;
    }
    
    public function importaProfissionaisMaisMedicosWS()
    {
    	$this->getProfissionaisWS();
    	if($this->arrProfissionais){
    
    		$muncods = $this->carregar("select muncod from territorios.municipio");
    		if($muncods){
    			foreach($muncods as $dados){
    				$arMuncod[substr($dados['muncod'], 0, 6)] = $dados['muncod'];
    			}
    		}
    
    		// INATIVA PROFISSIONAIS ANTES DE INSERIR CARGA NOVA DO WS
    		$this->inativaProfissionaisCadastrados();
    		
    		foreach($this->arrProfissionais->item as $k => $arrSup){
    			 
    			//Transforma o objeto em array
    			$arrSup = (array)$arrSup;
    
    			// TRATA VALORES NULOS
    			foreach($arrSup as $k => $v){
    				$arrSup[$k] = empty($arrSup[$k]) ? null : $arrSup[$k];
    			}
    			 
    			// TRATA CAMPO DE TELEFONE
    			if(!empty($arrSup['deFone'])){
    				$arfoneTmp = strpos( $arrSup['deFone'], '|' ) !== false ? explode('|', $arrSup['deFone']) : $arrSup['deFone'];
    				$arrSup['deFone'] = is_array($arfoneTmp) ? $arfoneTmp[0] : $arfoneTmp;
    			}
    			 
    			// FORCA STATUS ATIVO
    			$arrSup['wssstatus'] = 'A';
    			 
    			// Atualiza o numero do ciclo
    			$arrSup['nuciclo'] = null;
    			if (strpos( strtolower($arrSup['deCicloEntrada']), 'primeiro ciclo' ) !== false)
    				$arrSup['nuciclo'] = 1;
    			if (strpos( strtolower($arrSup['deCicloEntrada']), 'segundo ciclo' ) !== false)
    				$arrSup['nuciclo'] = 2;
    			if (strpos( strtolower($arrSup['deCicloEntrada']), 'terceiro ciclo' ) !== false)
    				$arrSup['nuciclo'] = 3;
    			if (strpos( strtolower($arrSup['deCicloEntrada']), 'quarto ciclo' ) !== false)
    				$arrSup['nuciclo'] = 4;
    			if (strpos( strtolower($arrSup['deCicloEntrada']), 'quinto ciclo' ) !== false)
    				$arrSup['nuciclo'] = 5;
    			if (strpos( strtolower($arrSup['deCicloEntrada']), 'sexto ciclo' ) !== false)
    				$arrSup['nuciclo'] = 6;
    			 
    			 
    			// Atualiza o numero da etapa
    			$arrSup['nuetapa'] = null;
    			if (strpos( strtolower($arrSup['deCicloEntrada']), 'primeira etapa' ) !== false)
    				$arrSup['nuetapa'] = 1;
    			if (strpos( strtolower($arrSup['deCicloEntrada']), 'segunda etapa' ) !== false)
    				$arrSup['nuetapa'] = 2;
    			if (strpos( strtolower($arrSup['deCicloEntrada']), 'terceira etapa' ) !== false)
    				$arrSup['nuetapa'] = 3;
    			if (strpos( strtolower($arrSup['deCicloEntrada']), 'quarta etapa' ) !== false)
    				$arrSup['nuetapa'] = 4;
    			if (strpos( strtolower($arrSup['deCicloEntrada']), 'quinta etapa' ) !== false)
    				$arrSup['nuetapa'] = 5;
    			if (strpos( strtolower($arrSup['deCicloEntrada']), 'sexta etapa' ) !== false)
    				$arrSup['nuetapa'] = 6;
    			 
    			$arrSup['wssstatus'] 	= 'A';
    			$arrSup['muncod'] 		= $arMuncod[$arrSup['ibgeAtuacao']];
    			 
    			$this->popularDadosObjeto($arrSup);
    			$this->salvar();
    			$this->clearDados();
    				
    		}
    		$this->commit();
    	}
    }
    
    private function inativaProfissionaisCadastrados()
    {
    	$sql = "update maismedicos.ws_profissionais set wssstatus = 'I'
    			where to_char(wssdata,'YYYY-MM') = to_char(now(),'YYYY-MM') and wssstatus = 'A' ";
    	
    	$this->executar($sql);
    	$this->commit();
    }
    
    public function atualizarDadosProfissionaisMaisMedicos()
    {
    	 
    	//     	$sql = "update maismedicos.ws_profissionais m1 set
    	// 					uniid = m2.uniid
    	// 				from (
    
    	// 					select
    	// 						wssid,
    	// 						u.uniid
    	// 					from maismedicos.ws_profissionais p
    	// 					left join maismedicos.universidade u on removeacento(lower(u.uninome)) ilike removeacento(lower(p.instituicaosupervisora)) and u.unistatus = 'A'
    	// 					where wssstatus = 'A'
    	// 					and to_char(wssdata,'YYYY-MM-DD') = (select to_char(max(wssdata),'YYYY-MM-DD') from maismedicos.ws_profissionais)
    	//     				and (p.nuciclo is null or p.uniid is null)
    
    	// 				) as m2
    	// 				where m1.wssid = m2.wssid;";
    	 
    	//     	$this->executar($sql);
    	//     	$this->commit();
    	 
    	//     	$sql = "select count(distinct p.nucpf) from maismedicos.ws_profissionais p
    	//     			where to_char(wssdata,'YYYY-MM-DD') = (select to_char(max(wssdata),'YYYY-MM-DD') from maismedicos.ws_profissionais)
    	//     			and wssstatus = 'A'
    	//     			and p.uniid is not null";
    
    	//     	$total = $this->pegaUm($sql);
    	 
    	//     	$txt = $total.' profissionais vinculados a universidades.';
    	//     	echo $txt;
    	//     	return $txt;
    }
    
    private function recuperaProfissionaisCadastradosHoje()
    {
    	$sql = "select nucpf, idinstituicao from maismedicos.ws_profissionais
    			where wssstatus = 'A' ";
    	$arrDados = $this->carregar($sql);
    	if($arrDados){
    		foreach($arrDados as $d)
    		{
    			$arrSup[$d['idinstituicao']][] = $d['nucpf'];
    		}
    	}
    	return $arrSup ? $arrSup : array();
    }
}