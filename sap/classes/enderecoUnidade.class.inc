<?php

/**
 * Modelo de Endereços de unidade
 * @author Silas Matheus
 */
class EnderecoUnidade extends Modelo{
	
   /**
	* Nome da tabela de unidade
	* @name $stTabelaUnidade
	* @var string
	* @access protected
	*/
	protected $stTabelaUnidade = "siorg.unidadeorganizacional";
	
   /**
	* Nome da tabela de endereço
	* @name $stTabelaEndereco
	* @var string
	* @access protected
	*/
	protected $stTabelaEndereco = "siorg.endereco";
	
   /**
	* Nome da tabela de endereço/andar
	* @name $stTabelaEndAndar
	* @var string
	* @access protected
	*/
	protected $stTabelaEndAndar = "siorg.enderecoandar";
	
   /**
	* Nome da tabela de endereço/andar/sala
	* @name $stTabelaEndAndarSala
	* @var string
	* @access protected
	*/
	protected $stTabelaEndAndarSala = "siorg.enderecoandarsala";
	
   /**
	* Nome da tabela de unidade/endereço
	* @name $stTabelaUorgEnd
	* @var string
	* @access protected
	*/
	protected $stTabelaUorgEnd = "siorg.uorgendereco";

	
	/**
	 * Monta lista de endereços de acordo com a unidade
	 * @name filtrarEndereco
	 * @author Alysson Rafael
	 * @access public
	 * @param int $uorco_uorg_lotacao_servidor - Código da Unidade
	 * @param int $viaEntradaBens              - Indica se a chamada veio da tela de entrada de bens
	 * @return void
	 */
	public function filtrarEndereco($uorco_uorg_lotacao_servidor,$viaEntradaBens){
	
		$viaEntradaBens = addslashes($viaEntradaBens);
		
		$sql = "SELECT ";
		if($viaEntradaBens == 'S'){
			$sql .= "
			'<center>
				<a style=\"cursor:pointer;\" onclick=\"executarScriptPai(\'retornaEndereco(' || uorgend.uendid || ')\');self.close();\">
				<img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Retornar Com Estes Dados\" />
				</a>
			</center>' ";
		}
		else{
			$sql .= "
			'<center>
				<a style=\"cursor:pointer;\">
				</a>
			</center>' ";
		}
		$sql .= "   as comandos,
					SUBSTR(cast(endereco.endcep AS text) , 1 , 2)||'.'||SUBSTR(cast(endereco.endcep AS text) , 3 , 3)||'-'||SUBSTR(cast(endereco.endcep AS text) , 6 , 3) AS endcep,
					endereco.enduf,
					endereco.endcid,
					
					CASE 
						WHEN CHAR_LENGTH(CAST(endereco.endnum AS text)) > 1 THEN
							endereco.endlog || ', ' || endereco.endnum || ', ' || endereco.endcom
						ELSE
							endereco.endlog || ', ' || endereco.endcom 
					END as descricao, 	
					
					endandar.enadescricao,
					endandarsala.easdescricao||' ' AS easdescricao 
				FROM 
					$this->stTabelaEndereco endereco
				JOIN 
				    $this->stTabelaEndAndar endandar ON endereco.endid=endandar.endid
				JOIN
					$this->stTabelaEndAndarSala endandarsala ON endandar.enaid=endandarsala.enaid
				JOIN
					$this->stTabelaUorgEnd uorgend ON endandarsala.easid=uorgend.easid
				JOIN 
					$this->stTabelaUnidade unidade ON unidade.uorco_uorg_lotacao_servidor=uorgend.uorco_uorg_lotacao_servidor
				WHERE 1=1 ";
		
		if(!empty($uorco_uorg_lotacao_servidor)){
			$uorco_uorg_lotacao_servidor = addslashes($uorco_uorg_lotacao_servidor);
			$sql .= "AND uorgend.uorco_uorg_lotacao_servidor='".$uorco_uorg_lotacao_servidor."' ";
		}
			
		$sql .= " ORDER BY descricao ";
		
		$cabecalho = array("","CEP","UF","Cidade","Endereço","Andar","Sala");
		$this->monta_lista($sql, $cabecalho, 20, 10, false, "center",'S','','',array('center','center','center','center','center','center','center'));
	
	}
	
	
	/**
	 * Monta combo de endereço
	 * @name montaComboEndereco
	 * @author Alysson Rafael
	 * @access public
	 * @return void
	 */
	function montaComboEndereco(){
		
		$sql = "SELECT endereco.endid AS codigo,endereco.endcom AS descricao ";
		$sql .= "FROM $this->stTabelaEndereco endereco ";
		$sql .= "JOIN $this->stTabelaEndAndar andar ON endereco.endid=andar.endid ";
		$sql .= "JOIN $this->stTabelaEndAndarSala sala ON andar.enaid=sala.enaid ";
		$sql .= "GROUP BY endereco.endid,endereco.endcom ORDER BY endereco.endcom ";

		$resultado = $this->carregar($sql);
		if(!is_array($resultado)){
			$resultado = array();
		}
		$this->monta_combo('endid',$resultado,'S','Selecione...','carregarAndar(this.value);carregaDadosEnderecoPorEndereco','','','200','S','endid','',$endid);
		
	}
	
	/**
	 * Monta combo de andares por endereço
	 * @name montaComboAndar
	 * @author Alysson Rafael
	 * @access public
	 * @param int $endid - Código do endereço
	 * @return void
	 */
	function montaComboAndar($endid){
		
		$resultado = null;
		if(!empty($endid)){
			$sql = "SELECT endandar.enaid AS codigo,endandar.enadescricao AS descricao ";
			$sql .= "FROM $this->stTabelaEndAndar endandar WHERE endandar.endid='".$endid."' ";
			$sql .= "ORDER BY endandar.enadescricao ";
			$resultado = $this->carregar($sql);
		}
		
		if(!is_array($resultado)){
			$resultado = array();
		}
		$this->monta_combo('enaid',$resultado,'S','Selecione...','carregarSala','','','200','S','enaid','',$enaid);
		
	}
	
	/**
	 * Monta combo de salas por andar
	 * @name montaComboSala
	 * @author Alysson Rafael
	 * @access public
	 * @param int $enaid - Código do andar
	 * @return void
	 */
	function montaComboSala($enaid){
		
		$resultado = null;
		if(!empty($enaid)){
			$sql = "SELECT endandarsala.easid AS codigo,endandarsala.easdescricao AS descricao ";
			$sql .= "FROM $this->stTabelaEndAndarSala endandarsala WHERE endandarsala.enaid='".$enaid."' ";
			$sql .= "ORDER BY endandarsala.easdescricao ";
			$resultado = $this->carregar($sql);
		}
		
		if(!is_array($resultado)){
			$resultado = array();
		}
		$this->monta_combo('easid',$resultado,'S','Selecione...','','','','200','S','easid','',$easid);
		
	}
	
	
	/**
	 * Verifica se já existe relacionamento entre a unidade e o endereço selecionados
	 * @name existe
	 * @author Alysson Rafael
	 * @access public
	 * @param array $campos - Campos do form
	 * @return bool
	 */	
	public function existe($campos){
		
		$campos['easid'] = addslashes($campos['easid']);
		$campos['uorco_uorg_lotacao_servidor'] = addslashes($campos['uorco_uorg_lotacao_servidor']);
		
		$sql = "SELECT COUNT(*) AS qtd FROM $this->stTabelaUorgEnd ";
		$sql .= "WHERE easid='".$campos['easid']."' AND uorco_uorg_lotacao_servidor='".$campos['uorco_uorg_lotacao_servidor']."' ";
		
		$result = $this->carregar($sql);
 	   	$qtd = $result[0]['qtd'];
		
 	   	if($qtd > 0){
 	   		$resultado = true;
 	   	}
 	   	else{
 	   		$resultado = false;
 	   	}

		return $resultado;
		
	}
	
	/**
	 * Salva novo relacionamento entre a unidade e o endereço selecionados
	 * @name salvar
	 * @author Alysson Rafael
	 * @access public
	 * @param array $campos - Campos para inserir
	 * @return int/bool
	 */
	public function salvar($campos){
	
		$campos['easid'] = addslashes($campos['easid']);
		$campos['uorco_uorg_lotacao_servidor'] = addslashes($campos['uorco_uorg_lotacao_servidor']);
		
		$sql = "INSERT INTO $this->stTabelaUorgEnd ";
		$sql .= "(easid, uorco_uorg_lotacao_servidor) ";
		$sql .= "VALUES ('".$campos['easid']."','".$campos['uorco_uorg_lotacao_servidor']."') ";
		$sql .= "RETURNING uendid ";

		$uendid = $this->pegaUm($sql);
		if($uendid > 0){
			$this->commit(); 
			return $uendid;
		}
		else{
			return false;
		}
	}
	
	/**
	 * Monta grid de unidades através dos filtros informados
	 * @name filtrarUnidade
	 * @author Alysson Rafael
	 * @access public
	 * @param string $uorsg - Sigla da Unidade
	 * @param string $uorno - Descrição da Unidade
	 * @return void
	 */
	public function filtrarUnidade($uorsg, $uorno,$viaEntradaBens,$uorco_uorg_lotacao_servidor=''){
	
		$sql = "SELECT 
						'<center>
							<a style=\"cursor:pointer;\" onclick=\"recarregaEnderecos(\''||unidade.uorco_uorg_lotacao_servidor||'\',\''||unidade.uorsg||'\',\''||unidade.uorno||'\');\">
                    		<img src=\"/imagens/alterar.gif \" border=0 title=\"Recarregar Endereços\">
                    		</a>
						</center>'					
					as comandos,
					unidade.uorco_interno_uorg||' ' AS uorco_interno_uorg,
					unidade.uorsg, 
					unidade.uorno 
				FROM 
					$this->stTabelaUnidade unidade
				WHERE 1=1 ";
		
		if(!empty($uorsg)){
			$uorsg = addslashes($uorsg);
			$sql .= "AND unidade.uorsg ILIKE '%".$uorsg."%' ";
		}
			
		if(!empty($uorno)){	
			$uorno = addslashes($uorno);
			$sql .= "AND unidade.uorno ILIKE '%".$uorno."%' ";
		}	
		
		if(!empty($uorco_uorg_lotacao_servidor)){
			$uorco_uorg_lotacao_servidor = addslashes($uorco_uorg_lotacao_servidor);
			$sql .= "AND unidade.uorco_uorg_lotacao_servidor = '{$uorco_uorg_lotacao_servidor}' ";
		}
			
		$sql .= "ORDER BY unidade.uorno ";
				
		$cabecalho = array("Selecionar","Código","Sigla","Unidade");
		$this->monta_lista($sql, $cabecalho, 20, 10, false, "center",'S','','',array('center','center','center','center'));
	
	}
	
	
	/**
	 * Carrega os dados completos de um endereço pelo id da tabela uorgendereco
	 * @name carregaEndereco
	 * @author Alysson Rafael
	 * @access public
	 * @param int $uendid - Código da tabela uorgendereco
	 * @return array
	 */
	function carregaEndereco($uendid){
		
		$uendid = addslashes($uendid);
		
		$sql = "SELECT SUBSTR(cast(endereco.endcep AS text) , 1 , 2)||'.'||SUBSTR(cast(endereco.endcep AS text) , 3 , 3)||'-'||SUBSTR(cast(endereco.endcep AS text) , 6 , 3) AS endcep,";
		$sql .= "endereco.enduf,endereco.endcid,endereco.endbairro,endereco.endlog,endereco.endcom,endereco.endnum,";
		$sql .= "unidade.uorno,unidade.uorco_uorg_lotacao_servidor,uorgend.uendid,andar.enadescricao,sala.easdescricao ";
		$sql .= "FROM $this->stTabelaEndereco endereco ";
		$sql .= "LEFT JOIN $this->stTabelaEndAndar andar ON endereco.endid=andar.endid ";
		$sql .= "LEFT JOIN $this->stTabelaEndAndarSala sala ON andar.enaid=sala.enaid ";
		$sql .= "LEFT JOIN $this->stTabelaUorgEnd uorgend ON sala.easid=uorgend.easid ";
		$sql .= "LEFT JOIN $this->stTabelaUnidade unidade ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor ";
		$sql .= "WHERE uorgend.uendid='".$uendid."' ";
		
		$resultado = $this->carregar($sql);
		return $resultado[0];
		
	}
	
	
	
	/**
	 * Carrega os dados completos de um endereço pelo id do endereço
	 * @name carregaEnderecoPorEndereco
	 * @author Alysson Rafael
	 * @access public
	 * @param int $endid - Código do endereço
	 * @return array
	 */
	function carregaEnderecoPorEndereco($endid){
		
		$endid = addslashes($endid);
		
		$sql = "SELECT SUBSTR(cast(endereco.endcep AS text) , 1 , 2)||'.'||SUBSTR(cast(endereco.endcep AS text) , 3 , 3)||'-'||SUBSTR(cast(endereco.endcep AS text) , 6 , 3) AS endcep,";
		$sql .= "endereco.enduf,endereco.endcid,endereco.endbairro,endereco.endlog,endereco.endcom,endereco.endnum,";
		$sql .= "andar.enadescricao,sala.easdescricao ";
		$sql .= "FROM $this->stTabelaEndereco endereco ";
		$sql .= "LEFT JOIN $this->stTabelaEndAndar andar ON endereco.endid=andar.endid ";
		$sql .= "LEFT JOIN $this->stTabelaEndAndarSala sala ON andar.enaid=sala.enaid ";
		$sql .= "WHERE endereco.endid='".$endid."' ";
		
		$resultado = $this->carregar($sql);
		return $resultado[0];
		
	}
	
	
	
	
	
	
	
	/**
	 * Retorna a qtd de endereços associados a uma unidade
	 * @name contaEnderecosDaUnidade
	 * @author Alysson Rafael
	 * @access public
	 * @param string $co_uorg_lotacao_servidor - Identificador da unidade organizacional
	 * @return int
	 */
	function contaEnderecosDaUnidade($co_uorg_lotacao_servidor){
		
		$co_uorg_lotacao_servidor = addslashes($co_uorg_lotacao_servidor);
		
		$sqlUnidade = "SELECT COUNT(*) AS tot FROM $this->stTabelaUnidade unidade ";
		$sqlUnidade .= "JOIN $this->stTabelaUorgEnd uorgend ON unidade.uorco_uorg_lotacao_servidor=uorgend.uorco_uorg_lotacao_servidor ";
		$sqlUnidade .= "WHERE unidade.uorco_uorg_lotacao_servidor='".$co_uorg_lotacao_servidor."' ";
		$tot = $this->pegaUm($sqlUnidade);
		return $tot;
	}
	
	/**
	 * Retorna os dados do endereço que estão associados a uma unidade
	 * @name carregaEnderecosDaUnidade
	 * @author Alysson Rafael
	 * @access public
	 * @param string $co_uorg_lotacao_servidor - Identificador da unidade organizacional
	 * @return array
	 */
	function carregaEnderecosDaUnidade($co_uorg_lotacao_servidor){
		
		$co_uorg_lotacao_servidor = addslashes($co_uorg_lotacao_servidor);
		
		$sql = "SELECT SUBSTR(cast(endereco.endcep AS text) , 1 , 2)||'.'||SUBSTR(cast(endereco.endcep AS text) , 3 , 3)||'-'||SUBSTR(cast(endereco.endcep AS text) , 6 , 3) AS endcep,";
		$sql .= "endereco.enduf,endereco.endcid,endereco.endbairro,endereco.endlog,endereco.endcom,endereco.endnum,";
		$sql .= "unidade.uorno,unidade.uorco_uorg_lotacao_servidor,unidade.uorsg,uorgend.uendid,andar.enadescricao,sala.easdescricao ";
		$sql .= "FROM $this->stTabelaUnidade unidade ";
		$sql .= "LEFT JOIN $this->stTabelaUorgEnd uorgend ON unidade.uorco_uorg_lotacao_servidor=uorgend.uorco_uorg_lotacao_servidor ";
		$sql .= "LEFT JOIN $this->stTabelaEndAndarSala sala ON uorgend.easid=sala.easid ";
		$sql .= "LEFT JOIN $this->stTabelaEndAndar andar ON sala.enaid=andar.enaid ";
		$sql .= "LEFT JOIN $this->stTabelaEndereco endereco ON andar.endid=endereco.endid ";
		$sql .= "WHERE unidade.uorco_uorg_lotacao_servidor='".$co_uorg_lotacao_servidor."' ";
		
		$resultado = $this->carregar($sql);
		return $resultado;
	}
	
	
	
	/**
     * Gerar a combo de unidades organizacionais
     * @name montaComboUnidade
     * @author Alysson Rafael
     * @access public
     * @return void
     */
	public function montaComboUnidade($co_uorg_lotacao_servidor=''){
		
		if($co_uorg_lotacao_servidor != ''){
			$sql = "SELECT uorco_uorg_lotacao_servidor AS codigo,uorsg AS descricao FROM $this->stTabelaUnidade WHERE uorco_uorg_lotacao_servidor='".$co_uorg_lotacao_servidor."' ORDER BY uorsg ";
		}
		else{
			$sql = "SELECT uorco_uorg_lotacao_servidor AS codigo,uorsg AS descricao FROM $this->stTabelaUnidade ORDER BY uorsg ";
		}

		$resultado = $this->carregar($sql);
		
		if(!$resultado || count($resultado) < 1){
			$resultado = array();
		}
		
		
		$this->monta_combo("uorco_uorg_lotacao_servidor",$resultado,'S',"Todos...","setaNomeUnidade","","","200","N","uorco_uorg_lotacao_servidor","",$uorco_uorg_lotacao_servidor);
	}
	
	
	/**
     * Carrega o nome da unidade e o id da tabela uorgendereco
     * @name carregaNomeUnidade
     * @author Alysson Rafael
     * @access public
     * @return array
     */
	public function carregaNomeUnidade(){
		$sql = "SELECT unidade.uorno,uorgend.uendid ";
		$sql .= "FROM $this->stTabelaUnidade unidade ";
		$sql .= "LEFT JOIN siorg.uorgendereco uorgend ON unidade.uorco_uorg_lotacao_servidor=uorgend.uorco_uorg_lotacao_servidor ";
		$sql .= "WHERE unidade.uorco_uorg_lotacao_servidor='".$_POST['uorco_uorg_lotacao_servidor']."' ";
		$resultado = $this->carregar($sql);
		return $resultado;
	}
	
	
	
	
	public function carregaNomeSiglaUnidade($uorco_uorg_lotacao_servidor){
		$sql = "SELECT uorno,uorsg FROM $this->stTabelaUnidade WHERE uorco_uorg_lotacao_servidor='{$uorco_uorg_lotacao_servidor}' ";
		$resultado = $this->carregar($sql);
		return $resultado;
	}
	
	
}

