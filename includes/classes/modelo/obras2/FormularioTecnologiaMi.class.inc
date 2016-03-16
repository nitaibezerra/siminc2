<?php
	
class FormularioTecnologiaMi extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.formulariotecnologiami";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ftiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ftiid' => null, 
									  	'tmiid' => null, 
									  	'ftinome' => null, 
									  	'ftiordem' => null, 
									  	'ftistatus' => null, 
									  	'arqid' => null, 
									  	'ftisnobrigatorio' => null, 
									  );

	public function listaComArquivoPorObridSupid( $obrid, $supid ){
		$sql = "SELECT 
					ft.*,
					a.arqid AS arqidup,
					a.arqnome AS arqnomeup,
					a.arqextensao AS arqextensaoup
				FROM
					obras2.obras o
				JOIN entidade.endereco e ON e.endid = o.endid
				JOIN obras2.empresami_uf eu ON eu.estuf = e.estuf AND
							       			   eu.eufstatus = 'A'
				JOIN obras2.formulariotecnologiami ft ON ft.tmiid = eu.tmiid AND
									 					 ft.ftistatus = 'A'
				-- LEFT JOIN public.arquivo a ON a.arqid = ft.arqid
				
				LEFT JOIN obras2.arquivosupervisaoformulariotecnologiami aft ON aft.ftiid = ft.ftiid AND
																				aft.supid = {$supid} AND
																				aft.asfstatus = 'A'
				LEFT JOIN public.arquivo a ON a.arqid = aft.arqid
				WHERE
					o.obrid = {$obrid}
				ORDER BY
					ftiordem";
		$dados = $this->carregar( $sql );
		
		return ($dados ? $dados : array());
	}
    
    public function listaSql(array $params = array()) {
        $where = array();

        
        $and = (count($where) ? " AND " . implode(' AND ', $where) : "");


        $sql = <<<EOD
                
					SELECT DISTINCT
                        '<center>
                            <img
                                align=\"absmiddle\"
                                src=\"/imagens/alterar.gif\" border=\"0\" /
                                style=\"cursor: pointer; margin-left: 3px;\"
                                onclick=\"javascript:abrirPergunta(' || fti.ftiid || ');\"
                                />
                            <img
                                align=\"absmiddle\"
                                src=\"/imagens/excluir.gif\" border=\"0\" /
                                style=\"cursor: pointer; margin-left: 3px;\"
                                onclick=\"javascript:excluirPergunta(' || fti.ftiid || ');\"
                                />
                         </center>' as acao,
                        tmi.tminome,
                        fti.ftinome,
                        fti.ftiordem,
                        CASE WHEN arq.arqdescricao = '' THEN
                            '' 
                        ELSE
                            '<a href="javascript:downloadArquivo('|| fti.arqid ||')"> '|| arq.arqnome ||'.'||arq.arqextensao||'</a>'
                        END as arquivo,
                        CASE WHEN fti.ftisnobrigatorio IS TRUE THEN
                            'Sim'
                        ELSE
                            'Não'
                        END as obrigatorio
                        FROM obras2.formulariotecnologiami fti
                        JOIN obras2.tecnologiami tmi ON fti.tmiid = tmi.tmiid AND tmi.tmistatus = 'A'
                        LEFT JOIN public.arquivo arq on arq.arqid = fti.arqid
                        WHERE fti.ftistatus = 'A'
                        ORDER BY fti.ftiordem, tmi.tminome
					
					$and
EOD;
        return $sql;
    }
    
    public function salvarArquivoFormulario($ftiid = null,$campo = "arqid",$descricao = "Arquivo de Modelo")
	{
		if($ftiid)
            $this->excluirArquivo($ftiid);
		
        require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
        $file = new FilesSimec(null,null,"obras2");
        $file->setUpload($descricao,null,false,false);
        return $file->getIdArquivo();
	}
	
    
    public function salvarFormulario()
	{
		$arrDados['ftiid'] = $_REQUEST['ftiid'] ? $_REQUEST['ftiid'] : null;
		$arrDados['tmiid'] = $_REQUEST['tmiid'];
		$arrDados['ftinome'] = $_REQUEST['ftinome'];
		$arrDados['ftiordem'] = $_REQUEST['ftiordem'];
		$arrDados['ftisnobrigatorio'] = ($_REQUEST['ftisnobrigatorio']) ? 'true' : 'false';
		$arrDados['arqid'] = $_REQUEST['arquivo'] ? $_REQUEST['arquivo']:$this->salvarArquivoFormulario($arrDados['ftiid']);
		$this->popularDadosObjeto($arrDados);
		$ftiid = $this->salvar();
        $this->commit();
        
		if(!$ftiid){
			$_SESSION['obras2']['pagamento']['alert'] = "Não foi possível realizar a operação.";
        } else if($arrDados['ftiid']){
			$_SESSION['obras2']['pagamento']['alert'] = "Formulário atualizado com sucesso!";
        }else{
			$_SESSION['obras2']['pagamento']['alert'] = "Formulário inserido com sucesso!";
        }
		header("Location: obras2.php?modulo=principal/listaFormularioMI&acao=A");
		exit;
	}

    public function excluirFormulario($ftiid = null)
	{
		$ftiid = ($ftiid) ? $ftiid : $_REQUEST['ftiid'];

		$sql = "UPDATE {$this->stNomeTabela} SET ftistatus = 'I' WHERE ftiid = $ftiid";
		if($this->executar($sql)){
			$this->commit();
			$_SESSION['obras2']['pagamento']['alert'] = "Operação realizada com sucesso.";
		}else{
			$_SESSION['obras2']['pagamento']['alert'] = "Não foi possível realizar a operação.";	
		}
		header("Location: obras2.php?modulo=principal/listaFormularioMI&acao=A");
		die;
	}
    
    public function excluirArquivo($ftiid = null, $arqid = null)
	{
		$arqid = $_POST['arqid'];
		$campo = $_POST['campo'] ? $_POST['campo'] : "arqid";
        
        if($ftiid){
            $sql = "select arq.arqid from obras2.formulariotecnologiami fti
                    join public.arquivo arq on arq.arqid = fti.arqid
                    where fti.ftiid = $ftiid";
            $arqid = $this->pegaUm($sql);
        }
		//Fazer exclusão do arquivo físico?
        if($arqid){
            $sql = "update public.arquivo set arqstatus = 'I' where arqid = $arqid;
                    --update $this->stNomeTabela set $campo = null where $campo = $arqid";
            $this->executar($sql);
            $this->commit();
        }
	}
    
									  
}