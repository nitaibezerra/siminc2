<?php
	
class RemessaDetalheDevolucao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.remessadetalhedevolucao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rdeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'rdeid' => null, 
									  	'rmcid' => null, 
									  	'rmdid' => null, 
									  	'arqid' => null, 
									  	'nu_nib' => null, 
									  	'dt_fim_periodo' => null, 
									  	'dt_ini_periodo' => null, 
									  	'cs_natur_credito' => null, 
									  	'dt_mov_credito' => null, 
									  	'id_orgao_pagador' => null, 
									  	'dt_fim_validade' => null, 
									  	'dt_ini_validade' => null, 
									  	'tutid' => null, 
									  	'strid' => null, 
									  	'cs_unid_monet' => null, 
									  	'vl_credito' => null, 
									  	'dt_ocorrencia' => null, 
									  	'bo_email_enviado' => null, 
									  	'dt_envio_email' => null, 
									  	'dt_cadastro' => null, 
									  );
    
    public function devolverRemessaCredito()
    {
    	
    	// Pega conteudo do arquivo
    	$conteudo = file_get_contents($_FILES['arquivoremesssa']['tmp_name']);
    	 
    	// Separa as linhas em array
    	$arrDetalhe = explode("\n",$conteudo);
    	
    	// Verifica se o array existe
    	if(is_array($arrDetalhe)){
    			
    		// Percorre as linhas
    		foreach($arrDetalhe as $linha => $det){
    
    			$beneficio = substr($det, 10,10);
    
    			if( $linha==0 || $linha==(count($arrDetalhe)-2) || empty($beneficio) ) continue;
    
    			// Zera o array antes de
    			$arrTutor = false;
    
    			// Monta array com os dados
    			$arrTutor['nu_nib'] = substr($det, 10,10);
    			$arrTutor['rmdid'] = (int)  substr($det, 117,11);
    			$arrIds[] = (int)  substr($det, 117,11).(' -> '.$det);
    			$arrTutor['dt_fim_periodo'] = substr($det, 20,8);
    			$arrTutor['dt_ini_periodo'] = substr($det, 28,8);
    			$arrTutor['cs_natur_credito'] = substr($det, 36,2);
    			$arrTutor['dt_mov_credito'] = substr($det, 38,8);
    			$arrTutor['id_orgao_pagador'] = substr($det, 46,6);
    			$arrTutor['vl_credito'] = substr($det, 52,12);
    			$arrTutor['cs_unid_monet'] = substr($det, 64,1);
    			$arrTutor['dt_fim_validade'] = substr($det, 65,8);
    			$arrTutor['dt_ocorrencia'] = substr($det, 65,8);
    				
    			// Trata valores
    			$arrTutor['vl_credito'] = (float) substr($arrTutor['vl_credito'], 0, strlen($arrTutor['vl_credito'])-2).'.'.substr($arrTutor['vl_credito'], strlen($arrTutor['vl_credito'])-2, strlen($arrTutor['vl_credito']));
    
    			// Popula detalhes
    			$detalhe[] = $arrTutor;
    			//ver($det, $detalhe, d);
    			
    		}
    	}
    	
    	// Faz upload do arquivo
    	if($_FILES['arquivoremesssa']['tmp_name']){
    		
    		require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    		
    		$file = new FilesSimec();
    		
    		if( $_FILES['arquivoremesssa']['tmp_name'] ){
    			$arquivoSalvo = $file->setUpload('Devolução da remessa', "arquivoremesssa", false);
//     			$this->commit();
    			$arqid = $file->getIdArquivo();    			
    		}    		
    	}
    	
    	// Insere e atualiza dados
    	if(is_array($detalhe)){
    		    		    		
    		foreach($detalhe as $dados){
    			
    			$ano = substr($dados['dt_ini_periodo'], 0, 4);
    			$mes = substr($dados['dt_ini_periodo'], 4, 2);
    			
    			$sql = "select true from maismedicos.autorizacaopagamento 
    					where apgstatus = 'A' and apgano = '{$ano}' and apgmes = '{$mes}'
    					and rmdid is null
    					and apgcpf = (select distinct nu_cpf from maismedicos.remessadetalhe where rmdid = {$dados['rmdid']})";
    			
    			$existe = $this->pegaUm($sql);
    			
    			//if ($existe) ver($existe, $sql, $dados, d);
    			$existe = false;
    			if (!$existe) {
    				
	    			// Devolve a remessa deltalhe
	    			$sqlUpdate = "update maismedicos.remessadetalhe set
					    				cs_ocorrencia = '9999'
					    			where rmdid = {$dados['rmdid']};";
	    			
	    			$this->executar($sqlUpdate);
	    			
	    			// Recupera dados para inserir nova autorizacao de pagamento
	    			$rs = $this->pegaLinha("select * from maismedicos.remessadetalhe d
	    									left join maismedicos.autorizacaopagamento a on a.rmdid = d.rmdid
	    										and apgstatus = 'A'
	    									left join maismedicos.tutor t on t.tutcpf = d.nu_cpf
	    										and tutstatus = 'A'  
	    									where d.rmdid = {$dados['rmdid']};");
	    			
	    			// Insere nova autorizacao de pagamento
	    			if($rs){
	    				
	    				$apgcpf = $rs['apgcpf'] ? $rs['apgcpf'] : $rs['tutcpf'];
	    				$apgnome = $rs['apgnome'] ? $rs['apgnome'] : $rs['tutnome'];
	    				$apgtipo = $rs['apgtipo'] ? $rs['apgtipo'] : $rs['tuttipo'];
	    				$apgmes = $rs['apgmes'] ? $rs['apgmes'] : $mes;
	    				$apgano = $rs['apgano'] ? $rs['apgano'] : $ano;
	    				$apgcpfresponsavel = $rs['apgcpfresponsavel'] ? $rs['apgcpfresponsavel'] : $_SESSION['usucpf'];
	    				$apgnomeresponsavel = $rs['apgnomeresponsavel'] ? $rs['apgnomeresponsavel'] : $_SESSION['usunome'];
	    				$apgjustificativa = 'Devolução de remessa de crédito.';
	    				
						$sqlInsert = "insert into maismedicos.autorizacaopagamento
											(apgcpf,apgnome,apgtipo,apgmes,apgano,rmdid,apgcpfresponsavel,apgnomeresponsavel,apgstatus,
											apgjustificativa,apgenviado,apgdtregistro)
										values
											('{$apgcpf}', '{$apgnome}', '{$apgtipo}', '{$apgmes}', '{$apgano}', null, 
											'{$apgcpfresponsavel}','{$apgnomeresponsavel}', 'A', '{$apgjustificativa}', false, now());";
						$this->rollback();
		    			ver($dados['rmdid'], $sqlInsert, d);
						$this->executar($sqlInsert);
						
		    			$dados['rmdid'] = $rs['rmdid'];
		    			$dados['tutid'] = $rs['tutid'];
	    			}
	
	    			$dados['arqid'] = $arqid;
	    			$this->popularDadosObjeto($dados);
	    			$this->salvar();
	    			$this->clearDados();
    			}
    			
    		}
    		if($sqlUpdate){
	    		$this->commit();
	    		$this->sucesso('principal/devolverRemessaCredito');
    		}
    		//$this->insucesso('Já exite(m) autorização(ões) de pagamento ativa(s) para todos beneficiários desse lote!', '', 'principal/devolverRemessaCredito');
    		echo "<script>
    				alert('Já exite(m) autorização(ões) de pagamento ativa(s) para todos beneficiários desse lote!');
    				documento.location.href = '/maismedicos/maismedicos.php?modulo=principal/devolverRemessaCredito&acao=A';
    			</script>";
    		
    	}
    
    }
}