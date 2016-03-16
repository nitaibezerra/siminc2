<?php
	
class Tutor extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.tutor";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tutid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tutid' => null, 
									  	'tutcpf' => null,
    									'uniid' => null,
							    		'tutnome' => null,
							    		'tutdatanascimento' => null,
							    		'tutemail' => null,
							    		'tuttelefone' => null,
							    		'tutnomemae' => null,
							    		'tutcep' => null,
							    		'tutlogradouro' => null,
							    		'tutnumero' => null,
										'tutcomplemento' => null,
										'tutbairro' => null,
    									'estuf' => null,
    									'muncod' => null,
    									'bncid' => null,
    									'tutagencia' => null,
    									'tutconta' => null,
    									'tutstatus' => null,
    									'tuttipo' => null,
							    		'tutdataatualizacao' => null,
    									'tutnumbeneficio' => null
									  );
    
    public function salvarTutor()
    {
    	$_POST['tutcpf'] = str_replace(array(".","-"),array("",""),$_POST['tutcpf']);
    	$_POST['tutcep'] = str_replace(array(".","-"),array("",""),$_POST['tutcep']);
    	$_POST['tutdatanascimento'] = formata_data_sql($_POST['tutdatanascimento']);
    	$_POST['tutdataatualizacao'] = date("Y-m-d");
    	$this->popularDadosObjeto($_POST);
    	$tutid = $this->salvar();
    	if($tutid){
    		$this->commit();
    		$_SESSION['maismedicos']['tutor']['alert'] = "Operação realizada com sucesso.";
    	}else{
    		$_SESSION['maismedicos']['tutor']['alert'] = "Não foi possível realizar a operação.";
    	}
    	 
    	header("Location: maismedicos.php?modulo=principal/cadTutores&acao=A&tuttipo={$_POST['tuttipo']}");
    	exit;
    }
    
    public function listarTutores($uniid = null ,$tuttipo = "T")
    {
    	$arrPerfil = pegaPerfilGeral();
    	
    	if(in_array(PERFIL_CONSULTA,$arrPerfil) || in_array(PERFIL_APOIADOR,$arrPerfil) || in_array(PERFIL_TUTOR,$arrPerfil)){
    		$habilitado = "N";
    	}else{
    		$habilitado = "S";
    	}
    	
    	$uniid = $uniid ? $uniid : $_POST['uniid']; 
    	$tuttipo = $_POST['tuttipo'] ? $_POST['tuttipo'] : $tuttipo; 
    	if($uniid){
    		$arrWhere[] = "uniid = $uniid";
    	}
    	
    	if($habilitado == "S"){
    		$acao = "'<img src=\"../imagens/alterar.gif\" class=\"link\"  onclick=\"editarTutor(\'' || tutid || '\',\'' || tuttipo || '\')\"  />
    				  <img src=\"../imagens/excluir.gif\" class=\"link\"  onclick=\"excluirTutor(\'' || tutid || '\',\'' || tuttipo || '\')\"  />'";
    	}else{
    		$acao = "'<img src=\"../imagens/consultar.gif\" class=\"link\"  onclick=\"editarTutor(\'' || tutid || '\',\'' || tuttipo || '\')\"  />'";
    	}
    	    	 
    	$sql = "select
    				$acao as acao,
    				tutnome,
    				tutemail,
    				tuttelefone,
    				case when tutvalidade is true
    					then 'Sim'
    					else 'Não'
    				end as validado
			    from
			    	$this->stNomeTabela
			    where
			    	tutstatus = 'A'
			    and
			    	tuttipo = '$tuttipo'
			    ".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			    order by
			    	tutnome";
		$arrCab = array("Ação","Nome","E-mail","Telefone","Valido");
        $this->monta_lista_simples($sql, $arrCab, 10000,10000,"N");
    }
    
    public function excluirTutor()
    {
    	$tutid = $_POST['tutid'];
    	$uniid = $_POST['uniid'];
    	$tuttipo = $_POST['tuttipo'];
    	if($tutid){
    		$sql = "update $this->stNomeTabela set tutstatus = 'I' where tutid = $tutid";
    		$this->executar($sql);
    		$this->commit();
    	}
    	$this->listarTutores($uniid,$tuttipo);
    }
    
    function recuperaTutor()
    {
    	$tutcpf = $_POST['usucpf'];
    	$tutcpf = str_replace(array(".","-"), array("",""), $tutcpf);
    	
    	//Tabela de Tutores
    	$sql = "select 
    				*,
    				to_char(tutdatanascimento,'DD/MM/YYYY') as tutdatanascimento 
    			from 
    				maismedicos.tutor 
    			where 
    				tutcpf = '$tutcpf'";
    	$arrTutor = $this->pegaLinha($sql);
    	if($arrTutor){
    		foreach($arrTutor as $chave => $valor){
    			$arrDados[$chave] = utf8_encode($valor);
    		}
    		echo simec_json_encode($arrDados);
    		exit;
    	}
    	
    	//Tabela de Entidade
    	$sql = "select
					entnome as tutnome,
					to_char(entdatanasc,'DD/MM/YYYY') as tutdatanascimento,
					entemail as tutemail,
					'(' || entnumdddresidencial || ')' || entnumresidencial as tuttelefone,
					endcep as tutcep,
					endlog as tutlogradouro,
					endnum as tutnumero,
					endcom as tutcomplemento,
					ende.estuf,
					ende.muncod
					
				from
					entidade.entidade ent
				left join
					entidade.endereco ende ON ent.entid = ende.entid
		    	where
		    		entnumcpfcnpj = '$tutcpf'";
    	$arrEntidade = $this->pegaLinha($sql);
    	if($arrEntidade){
    		foreach($arrEntidade as $chave => $valor){
    			$arrDados[$chave] = utf8_encode($valor);
    		}
    		echo simec_json_encode($arrDados);
    		exit;
    	}
    	
    	//Receita Federal
    	require APPRAIZ . "www/includes/webservice/cpf.php";
    	
    	$objPessoaFisica = new PessoaFisicaClient("http://ws.mec.gov.br/PessoaFisica/wsdl");
    	$xml 			 = $objPessoaFisica->solicitarDadosPessoaFisicaPorCpf($tutcpf);
    	
    	$obj = (array) simplexml_load_string($xml);
    	if (!$obj['PESSOA']) {
    		die();
    	}
    	$pessoa   = (array) $obj['PESSOA'];
		$endereco = (array) $obj['PESSOA']->ENDERECOS->ENDERECO;
		$contato  = (array) $obj['PESSOA']->CONTATOS->CONTATO;

    	$arrReceita['tutnome'] = $pessoa['no_pessoa_rf'];
    	$arrReceita['tutnomemae'] = $pessoa['no_mae_rf'];
    	
    	if($pessoa['dt_nascimento_rf']){
    		$ano = substr($pessoa['dt_nascimento_rf'], 0,4);
    		$mes = substr($pessoa['dt_nascimento_rf'], 4,2);
    		$dia = substr($pessoa['dt_nascimento_rf'], 6,2);
    		$arrReceita['tutdatanascimento'] = "$dia/$mes/$ano";
    	}
    	$arrReceita['tutcep'] = $endereco['nu_cep'];
    	$arrReceita['tutlogradouro'] = $endereco['ds_logradouro'];
    	$arrReceita['tutnumero'] = $endereco['ds_numero'];
    	$arrReceita['tutcomplemento'] = $endereco['ds_logradouro_comp'];
    	$arrReceita['tutbairro'] = $endereco['ds_bairro'];
    	$arrReceita['estuf'] = $endereco['sg_uf'];
    	$arrReceita['muncod'] = str_replace("00000000","",$endereco['co_cidade']);
    	
    	if($arrReceita){
    		foreach($arrReceita as $chave => $valor){
    			$arrDados[$chave] = utf8_encode($valor);
    		}
    		echo simec_json_encode($arrDados);
    		exit;
    	}
    }
     
}