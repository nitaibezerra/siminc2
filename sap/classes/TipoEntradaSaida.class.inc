<?php

/**
 * Modelo de Tipo de Entrada/Saída
 * @author Alysson Rafael
 */
class TipoEntradaSaida extends Modelo{
	
	
	/**
	* Nome da tabela especificada
	* @name $stNomeTabela
	* @var string
	* @access protected
	*/
	protected $stNomeTabela = "sap.tipoentradasaida";

	/**
	* Chave primária
	* @name $arChavePrimaria
	* @var array
	* @access protected
	*/
	protected $arChavePrimaria = array('tesid');

	/**
	* Atributos da Tabela
	* @name $arAtributos
	* @var array
	* @access protected
	*/
	protected $arAtributos = array(
		'tesid' => null,
		'tdcid' => null,
		'tesdsc' => null,
		'testpo' => null,
		'tesslg' => null,
		'tesst' => null
	);
	
	/**
     * Gerar a combo de tipo de documento
     * @name montaComboTipoDocumento
     * @author Alysson Rafael
     * @access public
     * @param string $origem - De onde foi chamada a função
     * @return void
     */
	public function montaComboTipoDocumento($origem=''){
		
		$sqlTipoDoc = "SELECT tdcid AS codigo,tdcdsc AS descricao FROM sap.tipodocumento ORDER BY tdcdsc ";

		$resultadoTipoDoc = $this->carregar($sqlTipoDoc);

		if($origem == 'cadastro'){
			$this->monta_combo("tdcid",$resultadoTipoDoc,'S',"Todos...","","","","200","S","tdcid","",$tdcid);
		}
		else{
			$this->monta_combo("tdcid",$resultadoTipoDoc,'S',"Todos...","","","","200","N","tdcid","",$tdcid);
		}
		
	}
	
	/**
     * Listar tipos de entrada/saída através dos filtros informados
     * @name listarTipoEntradaSaidaPorFiltro
     * @author Alysson Rafael
     * @access public
     * @param string $tesslg - Sigla do tipo de entrada/saída
     * @param string $tesdsc - Descrição do tipo de entrada/saída
     * @param string $testpo - Tipo de entrada/saida
     * @param int $tdcid - Código do tipo de documento
     * @return void
     */
	public function listarTipoEntradaSaidaPorFiltro($tesslg,$tesdsc,$testpo,$tdcid){
		
		$where = " WHERE 1=1 ";
		if(!empty($tesslg)){
			$tesslg = addslashes($tesslg);
			$where .= " AND tipo.tesslg = '".$tesslg."' ";
		}
		if(!empty($tesdsc)){
			$tesdsc = addslashes($tesdsc);
			$where .= " AND tipo.tesdsc ILIKE '%".$tesdsc."%' ";
		}
		if(!empty($testpo)){
			$testpo = addslashes($testpo);
			if($testpo == 'T'){
				$where .= " AND (tipo.testpo = 'E' OR tipo.testpo = 'S') ";
			}
			else{
				$where .= " AND tipo.testpo = '".$testpo."' ";
			}
		}
		if(!empty($tdcid)){
			$tdcid = addslashes($tdcid);
			$where .= " AND tipo.tdcid='".$tdcid."' ";
		}
			
		$sql = "SELECT 
					
					CASE WHEN tipo.tesst=true THEN
		
					'<center>
                    <a style=\"cursor:pointer;\" onclick=\"detalharTipoEntradaSaida(\''||tipo.tesid||'\');\">
                    <img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
                    </a>
                    </center>'

                    ELSE
                    
                    '<center>
                    <a style=\"cursor:pointer;\" onclick=\"detalharTipoEntradaSaida(\''||tipo.tesid||'\');\">
                    <img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
                    </a>
                    <a style=\"cursor:pointer;\" onclick=\"' || CASE 
                    												WHEN (((SELECT COUNT(*) FROM sap.bens bens WHERE bens.tesid=tipo.tesid) = 0) AND ((SELECT COUNT(*) FROM sap.bensbaixa bensbaixa WHERE bensbaixa.tesid=tipo.tesid) = 0)) 
                    												THEN 'if(confirm(\'Deseja excluir o registro?\')){removerTipoEntradaSaida(\''||tipo.tesid||'\');}' 
                    												ELSE 'alert(\'Registro não pode ser excluído!\');' 
                    											END || '\">
                    <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                    </a>
                    </center>'
                    
                    END
                    
                    as acao,
                    tipo.tesslg as tesslg,
                    tipo.tesdsc as tesdsc,
                    tipodoc.tdcdsc,
                    CASE WHEN tipo.testpo='E' THEN 'Entrada' ELSE 'Saída' END 
                    FROM sap.tipoentradasaida tipo JOIN sap.tipodocumento tipodoc ON tipo.tdcid=tipodoc.tdcid ";
		$sql .= $where." ORDER BY tipo.tesdsc ";
		
		$cabecalho = array("Comando","Sigla","Descrição","Tipo de Documento","Tipo");
		$this->monta_lista($sql,$cabecalho,20,50,false,"center",'S','','',array('center','center','center','center','center'));
		
	}
		
	/**
     * Verificar se já existe uma descrição igual à digitada
     * @name verificaTipoRepetido
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return bool
     */
	public function verificaTipoRepetido($post){
		
		extract($post);
		
		$tesdsc = addslashes($tesdsc);
		
		//verifica se existe no banco um registro com a descrição informada no form
		$sql = "SELECT COUNT(*) AS tot FROM sap.tipoentradasaida WHERE tesdsc='".$tesdsc."' ";
		if(!empty($tesid)){
			$tesid = addslashes($tesid);
			$sql .= "AND tesid <> '".$tesid."' ";
		}
		
		$result = $this->carregar($sql);
		if($result[0]['tot'] >= 1){
			return true;
		}
		else{
			return false;
		}
		
	}
	
	/**
     * Cadastrar/editar tipos de entrada/saída
     * @name salvarTipoEntradaSaida
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return void
     */
	public function salvarTipoEntradaSaida($post){
		extract($post);
		
		$tesdsc = addslashes($tesdsc);
		$testpo = addslashes($testpo);
		$operacao = '';
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($tesid)){
			$operacao = 'adicionar';
			$sql = "INSERT INTO sap.tipoentradasaida(tdcid,tesdsc,testpo,tesslg) VALUES('".$tdcid."','".$tesdsc."','".$testpo."','".$tesslg."') ";
		}
		else{
			$tesid = addslashes($tesid);
			$operacao = 'editar';
			$sql = "UPDATE sap.tipoentradasaida SET tdcid='".$tdcid."',tesdsc='".$tesdsc."',testpo='".$testpo."',tesslg='".$tesslg."' WHERE tesid='".$tesid."' ";
		}
		
		
		$sql .= " returning tesid ";
       	$tesid = $this->pegaUm($sql);

       	if(!empty($tesid)){
       		$this->commit();
       		return array(0=>true,1=>$tesid,2=>$operacao);
       	}
       	else{
       		return array(0=>false,1=>$tesid,2=>$operacao);
       	}
	}
	
	/**
     * Carregar um tipo de entrada/saída pelo id
     * @name carregaTipoEntradaSaidaPorId
     * @author Alysson Rafael
     * @access public
     * @param int $tesid - Id a ser pesquisado
     * @return array
     */
	public function carregaTipoEntradaSaidaPorId($tesid){
		
		$tesid = addslashes($tesid);
		
		$sql = "SELECT tipo.tesid,tipo.tdcid,tipo.tesdsc,tipo.testpo,tipo.tesslg,tipo.tesst FROM sap.tipoentradasaida tipo WHERE tipo.tesid='".$tesid."' ";
        return $this->pegaLinha($sql);
		
	}
		
	/**
     * Excluir um tipo de entrada/saída
     * @name excluirTipoEntradaSaida
     * @author Alysson Rafael
     * @access public
     * @param int $tesid - Id do registro a ser excluído
     * @return void
     */
	public function excluirTipoEntradaSaida($tesid){
		
		$tesid = addslashes($tesid);
		
		$sql = "DELETE FROM sap.tipoentradasaida WHERE tesid='".$tesid."' ";
		$this->executar($sql);
		$this->commit();
        direcionar('?modulo=sistema/tabelasdeapoio/tipoEntradaSaida/pesquisar&acao=A','Operação realizada com sucesso!');
	}
		
	/**
     * Gerar a combo de tipo de entrada/saída
     * @name montaComboTipoEntradaSaida
     * @author Alysson Rafael
     * @access public
     * @param int    $tesid        - Id que receberá o selected
     * @param string $entradasaida - Indica se serão carregados os tipos de entrada, saída ou todos
     * @param string $obrigatorio     - Indica se o campo é obrigatório
     * @param string $verificaTipoEntradaSaida - Indica se vai usar a função verificaTipoEntradaSaida que fica no onchange do campo
     * @param string $habilita - Indica se o campo é disabled
     * @param string $nome - Indica o nome/id a ser usado no campo
     * @return void
     */
	public function montaComboTipoEntradaSaida($tesid = '',$entradasaida='', $obrigatorio = 'S',$verificaTipoEntradaSaida='N',$habilita='S',$nome='tesid'){
		
		//caso tenha sido informado este filtro
		if(!empty($entradasaida)){
			$entradasaida = addslashes($entradasaida);
			$sql = "SELECT tesid AS codigo,tesdsc AS descricao FROM sap.tipoentradasaida WHERE testpo='".$entradasaida."' ORDER BY tesdsc ";
		}
		else{
			$sql = "SELECT tesid AS codigo,tesdsc AS descricao FROM sap.tipoentradasaida ORDER BY tesdsc ";
		}

		$resultado = $this->carregar($sql);

		//caso seja para colocar a função verificaTipoEntradaSaida no evento onchange
		if($verificaTipoEntradaSaida == 'S'){
			$this->monta_combo($nome,$resultado, $habilita,"Selecione...","verificaTipoEntradaSaida","","","200", $obrigatorio,$nome,"",$tesid);
		}
		else if($verificaTipoEntradaSaida == 'N'){
			$this->monta_combo($nome,$resultado, $habilita,"Selecione...","","","","200", $obrigatorio,$nome,"",$tesid);
		}
		
	}	
}