<?php

/**
 * Modelo de Empenho
 * @author Alysson Rafael
 */
class Empenho extends Modelo{


	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sap.empenho";

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("empid");

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'empid' => null,
		'empnumero' => null,
		'empdata' => null,
		'empcidade' => null,
		'empvalorper' => null,
		'empvalortotal' => null
	);

	/**
	 * Lista empenhos através dos filtros informados
	 * @name listarEmpenhoPorFiltro
	 * @author Alysson Rafael
	 * @access public
	 * @param int $empnumero - Número do empenho
	 * @param string $empdata - Data do empenho
	 * @return void
	 */
	public function listarEmpenhoPorFiltro($empnumero,$empdata){

		$where = " WHERE 1=1 ";
		if(!empty($empnumero)){
			$empnumero = addslashes($empnumero);
			$where .= " AND empenho.empnumero = '".$empnumero."' ";
		}
		if(!empty($empdata)){
			$empdata = addslashes($empdata);
			$empdata = formata_data_sql($empdata);
			$where .= " AND empenho.empdata = '".$empdata."' ";
		}

		
		$sql = "SELECT '<center>
                  <a style=\"cursor:pointer;\" onclick=\"detalharEmpenho(\''||empenho.empid||'\');\">
                  <img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
                  </a>
                  <a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM sap.bens bens WHERE bens.empid=empenho.empid) = 0 THEN ' if(confirm(\'Deseja excluir o registro?\')){removerEmpenho(\''||empenho.empid||'\');}' ELSE 'alert(\'A Nota de Empenho '||empenho.empnumero||' não pôde ser excluída pois existem processos de Entrada de Bens relacionados à ela.\');' END || '\">
                  <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                  </a>
                  </center>' as acao,
                  empenho.empnumero||' ' as empnumero,
                  '<span style=\"display:none;\">' || empenho.empdata || '</span>' || to_char(empenho.empdata, 'DD/MM/YYYY') as empdata,
                  empenho.empvalorper as empvalorper,
                  empenho.empvalortotal as empvalortotal,
                  sum(empenho.empvalorper + empenho.empvalortotal) as valortotal
                  FROM sap.empenho empenho ".$where." 
                  GROUP BY empenho.empid,empenho.empnumero,empenho.empdata,empenho.empvalorper,empenho.empvalortotal 
                  ORDER BY empenho.empdata DESC ";
		
		$cabecalho = array("Comando","Número do Empenho","Data do Empenho","Valor Para Material Permanente","Valor Para Bens de Consumo","Valor Total");
		$this->monta_lista($sql,$cabecalho,20,50,false,"center",'S','','',array('center','center','center','right','right','right'));

	}

	/**
	 * Cadastrar/editar empenho
	 * @name salvarEmpenho
	 * @author Alysson Rafael
	 * @access public
	 * @param array $post - Dados do formulário
	 * @return array
	 */
	public function salvarEmpenho($post){

		extract($post);

		$empnumero = addslashes($empnumero);
		$operacao = '';

		//formata a data
		$empdata = formata_data_sql($empdata);

		//caso os valores venham vazios, seta com 0.00
		if(empty($empvalorper)){
			$empvalorper = '0.00';
		}
		else{
			$empvalorper = str_replace('.', '', $empvalorper);
			$empvalorper = str_replace(',', '.', $empvalorper);
			$empvalorper = addslashes($empvalorper);
		}
		if(empty($empvalortotal)){
			$empvalortotal = '0.00';
		}
		else{
			$empvalortotal = str_replace('.', '', $empvalortotal);
			$empvalortotal = str_replace(',', '.', $empvalortotal);
			$empvalortotal = addslashes($empvalortotal);
		}

		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($empid)){
			$operacao = 'adicionar';
			$sql = "INSERT INTO sap.empenho(empnumero,empdata,empvalorper,empvalortotal) ";
			$sql .= "VALUES('".$empnumero."','".$empdata."','".$empvalorper."','".$empvalortotal."') ";
		}
		else{
			$empid = addslashes($empid);
			$operacao = 'editar';
			$sql = "UPDATE sap.empenho SET empnumero='".$empnumero."',empdata='".$empdata."',";
			$sql .= "empvalorper='".$empvalorper."',empvalortotal='".$empvalortotal."' ";
			$sql .= "WHERE empid='".$empid."' ";
		}

		$sql .= " returning empid ";

		$empid = $this->pegaUm($sql);

		if(!empty($empid)){
			$this->commit();
			return array(0=>true,1=>$empid,2=>$operacao);
		}
		else{
			return array(0=>false,1=>$empid,2=>$operacao);
		}

	}

	/**
	 * Carregar um empenho pelo id
	 * @name carregaEmpenhoPorId
	 * @author Alysson Rafael
	 * @access public
	 * @param int $empid - Id a ser pesquisado
	 * @return array
	 */
	public function carregaEmpenhoPorId($empid){

		$empid = addslashes($empid);
		
		$sql = "SELECT empnumero,empdata,empvalorper,empvalortotal,sum(empvalorper + empvalortotal) as total ";
		$sql .= "FROM sap.empenho WHERE empid='".$empid."' GROUP BY empnumero,empdata,empvalorper,empvalortotal ";
		return $this->pegaLinha($sql);

	}

	/**
	 * Excluir um empenho
	 * @name excluirEmpenho
	 * @author Alysson Rafael
	 * @access public
	 * @param int $empid - Id do registro a ser excluído
	 * @param string $popup - Indica se a chamada foi do popup
	 * @return void
	 */
	public function excluirEmpenho($empid,$popup='N'){
		
		$empid = addslashes($empid);
		
		$sql = "DELETE FROM sap.empenho WHERE empid='".$empid."' ";
		$this->executar($sql);
		$this->commit();
		if($popup == 'N'){
			direcionar('?modulo=principal/empenho/pesquisar&acao=A','Operação realizada com sucesso!');
		}
		else if($popup == 'S'){
			direcionar('?modulo=principal/empenho/pesquisarEmpenho&acao=A&popup=S','Operação realizada com sucesso!');
		}
		
	}

	/**
	 * Monta sql do filtro de empenho
	 * @name filtrarEmpenho
	 * @author Alysson Rafael
	 * @access public
	 * @param int    $empnumero - Número do empenho a ser pesquisado
	 * @param string $empdata   - Data do empenho a ser pesquisada
	 * @return void
	 */
	public function filtrarEmpenho($empnumero,$empdata){

		$sql = "SELECT
					'<center>
						<a style=\"cursor:pointer;\" onclick=\"selecionar(\''||empenho.empid||'\');\">
                    	<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
                    	</a>
                    	<a style=\"cursor:pointer;\" onclick=\"detalharEmpenho(\''||empenho.empid||'\');\">
                    	<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
                    	</a>
                    	<a style=\"cursor:pointer;\" onclick=\"' || CASE WHEN(SELECT COUNT(*) FROM sap.bens bens WHERE bens.empid=empenho.empid) = 0 THEN ' if(confirm(\'Deseja excluir o registro?\')){removerEmpenho(\''||empenho.empid||'\');}' ELSE 'alert(\'Registro não pode ser excluído!\');' END || '\">
                    	<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                    	</a>	
					</center>'			
					as comandos,
					empenho.empnumero||' ' AS empnumero,
					'<span style=\"display:none;\">' || empenho.empdata || '</span>' || TO_CHAR(empenho.empdata, 'DD/MM/YYYY') AS empdata,
					empenho.empvalorper,
					empenho.empvalortotal,
					(empenho.empvalorper+empenho.empvalortotal) total 
				FROM 
				$this->stNomeTabela empenho
				WHERE 1=1 ";

		if(!empty($empnumero)){
			$empnumero = addslashes($empnumero);
			$sql .= "AND empenho.empnumero='".$empnumero."' ";
		}
		if(!empty($empdata)){
			$empdata = addslashes($empdata);
			$empdata = formata_data_sql($empdata);
			$sql .= "AND empenho.empdata='".$empdata."' ";
		}
		
		$sql .= "ORDER BY empenho.empdata DESC ";

		$cabecalho = array("Comando","Número do Empenho","Data do Empenho","Valor Para Material Permanente","Valor Para Bens de Consumo", "Valor Total");
		$this->monta_lista($sql, $cabecalho, 20, 50, false, "center",'S','','',array('center','center','center','right','right','right'));

	}

	/**
	 * Recupera registro
	 * @name pegarRegistro
	 * @author Alysson Rafael
	 * @access public
	 * @param int $id - Identificador do empenho
	 * @return array
	 */
	public function pegarRegistro($id){

		$id = addslashes($id);
		
		$sql = "SELECT
					* 
				FROM 
				$this->stNomeTabela
				WHERE 
					empid=$id";
				$return = $this->carregar($sql);

				return $return[0];

	}

	/**
	 * Pega o valor do empenho permanente
	 * @name pegarValorEmpenhoPermanente
	 * @author Silas Matheus
	 * @access public
	 * @param int $empid - Id do Empenho
	 * @return float
	 */
	public function pegarValorEmpenhoPermanente($empid){

		$empid = addslashes($empid);
		
		$sql = "SELECT
					empvalorper 
				FROM 
				$this->stNomeTabela
				WHERE 
					empid=$empid";
					
				return $this->pegaUm($sql);

	}

	/**
	 * Calcula se o valor do empenho permanente permite mais entradas de bens 
	 * @name pegarValorEmpenhoPermanente
	 * @author Silas Matheus
	 * @access public
	 * @param int $empid - Id do Empenho
	 * @param float $benvlrdoc - Valor do Documento
	 * @param int $benid - Id do Bem
	 * @return string
	 */
	public function validarValorEmpenhoPermanente($empid, $benvlrdoc, $benid = '',$benstatus=''){

		$empid = addslashes($empid);
		$benvlrdoc = addslashes($benvlrdoc);
		$benid = addslashes($benid);
		
		$erro = '';
		
		// soma o valor total dos documentos para este empenho
		$oBens = new Bens();
		$totalDocumentos = $oBens->somarValoresTotaisDocumentos($empid);

		// recupera o valor total do empenho permanente
		$valorEmpenhoPermanente = $this->pegarValorEmpenhoPermanente($empid);
		

		// soma o valor requerido 
		if(strpos($benvlrdoc,'.') !== false){
			$benvlrdoc = str_replace('.','',$benvlrdoc);
		}
		
		$soma = new Math($totalDocumentos, $benvlrdoc,8);
		$totalDocumentos = $soma->sum()->getResult();
		
				
		// se passado o id do bem
		if(!empty($benid) && $benstatus == 'A'){
			
			// recupera o valor do documento do bem
			$valorBem = $oBens->pegarValorDocumento($benid);
			
			// subtrai o valor deste bem no valor total
			$subtrai = new Math($totalDocumentos, $valorBem);
			$totalDocumentos = $subtrai->sub()->getResult();
		
		}
		
		// valida se o valor do empenho é maior ou igual ao total do documento
		$validacao = new Math($valorEmpenhoPermanente, $totalDocumentos,8);
		if($validacao->isLess()){
			$erro = 'less';
		}
			
			
		return $erro;

	}
	
	
	/**
	 * Verifica se um determinado empenho está vinculado a algum registro de entrada de bem 
	 * @name verificaEmpenhoEmUso
	 * @author Alysson Rafael
	 * @access public
	 * @param int $empid - Id do Empenho
	 * @return bool
	 */
	public function verificaEmpenhoEmUso($empid){
		$sql = "SELECT bem.benid FROM sap.bens bem WHERE bem.empid='".$empid."' AND bem.benstatus='A' ";
		$benid = $this->pegaUm($sql);
		if($benid > 0){
			return true;
		}
		else{
			return false;
		}
	}
	
	
	/**
	 * Verifica se um determinado número de empenho já está cadastrado 
	 * @name verificaEmpenhoRepetido
	 * @author Alysson Rafael
	 * @access public
	 * @param array $_POST - Array com os dados
	 * @return bool
	 */
	public function verificaEmpenhoRepetido($_POST){
		if(!empty($_POST['empnumero'])){
			$sql = "SELECT COUNT(*) AS qtd FROM sap.empenho WHERE empnumero='".$_POST['empnumero']."' ";
			
			if(!empty($_POST['empid'])){
				$sql .= "AND empid <> '".$_POST['empid']."' ";
			}
			
			$qtd = $this->pegaUm($sql);
			if($qtd >= 1){
				return true;
			}
			else{
				return false;
			}
		}
	}
	
	
	/**
	 * Valida a data e o valor de empenho de acordo com as entradas de bem registradas para o empenho 
	 * @name validaEdicao
	 * @author Alysson Rafael
	 * @access public
	 * @param array $_POST - Array com os dados
	 * @return array
	 */
	public function validaEdicao($_POST){
		
		$empdata = strtotime(formata_data_sql($_POST['empdata']));
		$erroData = false;
		
		$sql = "SELECT bendtentrada FROM sap.bens WHERE empid='".$_POST['empid']."' ";
		$resultado = $this->carregar($sql);
		if(is_array($resultado) && count($resultado) >= 1){
			foreach($resultado[0] as $key => $value){
				if($empdata > strtotime($value)){
					$erroData = true;
				}
			}	
		}
		
		$_POST['empvalorper'] = str_replace('.','',$_POST['empvalorper']);
		$erroValor = false;
		$oBens = new Bens();
		$valorTotal = $oBens->somarValoresTotaisDocumentos($_POST['empid']);
		$validacao = new Math($_POST['empvalorper'], $valorTotal);
		if($validacao->isLess()){
			$erroValor = true;
		}

		return array(0=>$erroData,1=>$erroValor);
		
	}
	
	
	public function carregarEmpenhoPorNumero($empnumero){
		$empnumero = addslashes($empnumero);
		
		$sql = "SELECT empid ";
		$sql .= "FROM $this->stNomeTabela WHERE empnumero='".$empnumero."' ";
		return $this->pegaLinha($sql);
	}
	

}