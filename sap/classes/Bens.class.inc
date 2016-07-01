<?php

/**
 * Modelo de Bem
 * @author Alysson Rafael
 */
class Bens extends Modelo{


	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sap.bens";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('benid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos	= array(
		'benid' => null,
		'tesid' => null,
		'empid' => null,
		'empid' => null,
		'bendtentrada' => null,
		'bendtproc' => null,
		'bennumdoc' => null,
		'bendtdoc' => null,
		'benitdoc' => null,
		'bencnpjceddoa' => null,
		'benceddoa' => null,
		'benobs' => null,
		'benstatus' => null,
		'benvlrdoc' => null,
		'nu_matricula_siape' => null,
		'benjust' => null,
		'uendid' => null,
		'usucpf' => null,
		'ternum' => null,
		'dscdoc' => null,
		'bennumproc' => null,
		'forcpfcnpj' => null,
		'ternumanterior' => null
	);


	/**
     * Cadastrar/editar bens
     * @name salvarBem
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return array
     */
	public function salvarBem($post){

		extract($post);


		$benobs = addslashes($benobs);
		$operacao = '';

		//formata as datas
		if(!empty($bendtdoc)){
			$bendtdoc = formata_data_sql($bendtdoc);
		}
		else{
			$bendtdoc = 'null';
		}

		if(!empty($bendtentrada)){
			$bendtentrada = formata_data_sql($bendtentrada);
		}
		else{
			$bendtentrada = 'null';
		}

		if(!empty($bendtproc)){
			$bendtproc = "'".formata_data_sql($bendtproc)."'";
		}
		else{
			$bendtproc = 'null';
		}

		//retira os caracteres do campo pois é numérico
		$bennumproc = str_ireplace('.', '', $bennumproc);
		$bennumproc = str_ireplace('-', '', $bennumproc);

		//caso estes campos estejam vazios, seta com null
		//caso não estejam, retira os caracteres
		if(empty($forcpfcnpj)){
			$forcpfcnpj = 'null';
		}
		else{
			$forcpfcnpj = str_ireplace('.', '', $forcpfcnpj);
			$forcpfcnpj = str_ireplace('/', '', $forcpfcnpj);
			$forcpfcnpj = str_ireplace('-', '', $forcpfcnpj);
		}

		if(empty($bencnpjceddoa)){
			$bencnpjceddoa = 'null';
		}
		else{
			$bencnpjceddoa = str_ireplace('.', '', $bencnpjceddoa);
			$bencnpjceddoa = str_ireplace('/', '', $bencnpjceddoa);
			$bencnpjceddoa = str_ireplace('-', '', $bencnpjceddoa);
		}

		if(empty($empid)){
			$empid = 'null';
		}

		$benvlrdoc = formata_valor_sql($benvlrdoc);


		if(empty($uendid)){
			$uendid = 'NULL';
		}

		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($benid)){
			$operacao = 'adicionar';
			$sql = "INSERT INTO sap.bens(tesid,forcpfcnpj,empid,bendtentrada,bennumproc,bendtproc,bennumdoc,bendtdoc,benvlrdoc,";
			$sql .= "bencnpjceddoa,benceddoa,benobs,benstatus, nu_matricula_siape, uendid,dscdoc) ";
			$sql .= "VALUES('".$tesid."','".$forcpfcnpj."',".$empid.",'".$bendtentrada."','".$bennumproc."',".$bendtproc.",";
			$sql .= $bennumdoc.",'".$bendtdoc."','".$benvlrdoc."',".$bencnpjceddoa.",'".$benceddoa."','".$benobs."','I', '$nu_matricula_siape', $uendid,'$dscdoc') ";
		}
		else{
			$operacao = 'editar';
			$sql = "UPDATE sap.bens SET bennumdoc=".$bennumdoc.",bendtdoc='".$bendtdoc."',benvlrdoc='".$benvlrdoc."',benobs='".$benobs."',dscdoc='".$dscdoc."',empid=$empid,bennumproc='".$bennumproc."'";
			if(!empty($forcpfcnpj)){
				$sql .= ",forcpfcnpj='".$forcpfcnpj."'";
			}

			if(!empty($bencnpjceddoa)){
				$sql .= ",bencnpjceddoa=".$bencnpjceddoa.",benceddoa='".$benceddoa."'";
			}

			$sql .= "WHERE benid='".$benid."' ";
		}

		$sql .= " RETURNING benid ";

		//executa a query com este método para gravar log
		$benid = $this->pegaUm($sql);

       	if(!empty($benid)){
       		$this->commit();

       		//grava um registro na tabela de histórico
       		if($operacao == 'editar'){
       			$oBensHistorico = new BensHistorico();
				$oBensHistorico->gravar($benjust, 'NULL',$benid);
       		}

       		return array(0=>true,1=>$benid);
       	}
       	else{
       		return array(0=>false,1=>$benid);
       	}
	}


	/**
     * Carregar um bem pelo id
     * @name carregaBemPorId
     * @author Alysson Rafael
     * @access public
     * @param int $benid - Id a ser pesquisado
     * @return array
     */
	public function carregaBemPorId($benid){

		$benid = addslashes($benid);

		$sql = "SELECT
					bem.tesid,
					TO_CHAR(bem.bendtentrada, 'DD/MM/YYYY') AS bendtentrada,
					bem.bennumproc,
					TO_CHAR(bem.bendtproc, 'DD/MM/YYYY') AS bendtproc,
					bem.bennumdoc,
					TO_CHAR(bem.bendtdoc, 'DD/MM/YYYY') AS bendtdoc,
					bem.benvlrdoc,
					bem.benitdoc,
					bem.bencnpjceddoa,
					bem.benceddoa,
					bem.benobs,
					bem.benstatus,
					bem.uendid,
					bem.dscdoc,
					tipoentsaid.tesslg,
					responsavel.nu_matricula_siape,
					responsavel.no_servidor,
					unidade.uorno AS unidade,
					endereco.enduf,
					endereco.endcid,
					SUBSTR(cast(endereco.endcep AS text) , 1 , 2)||'.'||SUBSTR(cast(endereco.endcep AS text) , 3 , 3)||'-'||SUBSTR(cast(endereco.endcep AS text) , 6 , 3) AS endcep,
					endereco.endlog,
					andar.enadescricao,
					sala.easdescricao,
					empenho.empid,
					empenho.empnumero,
					TO_CHAR(empenho.empdata, 'DD/MM/YYYY') AS empdata,
					empenho.empvalorper,
					
					CASE 
						WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 14 THEN
							SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 2)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 3 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 6 , 3)||'/'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 9 , 4)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 13 , 2) 
						WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 11 THEN
							SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 4 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 7 , 3)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 10 , 2)
						ELSE CAST(fornecedor.forcpfcnpj AS text)
					END,
					fornecedor.fornomefantasia
				FROM
					sap.bens bem
				INNER JOIN
				    sap.tipoentradasaida tipoentsaid
					ON bem.tesid=tipoentsaid.tesid
				LEFT JOIN
					sap.empenho empenho
					ON bem.empid=empenho.empid
				LEFT JOIN
					sap.fornecedor fornecedor
					ON bem.forcpfcnpj=fornecedor.forcpfcnpj
				INNER JOIN
					siape.vwservidorativo responsavel
					ON bem.nu_matricula_siape=responsavel.nu_matricula_siape
				LEFT JOIN
					siorg.uorgendereco uorgend
					ON bem.uendid=uorgend.uendid
				LEFT JOIN
					siorg.unidadeorganizacional unidade
					ON uorgend.uorco_uorg_lotacao_servidor=unidade.uorco_uorg_lotacao_servidor
				LEFT JOIN
					siorg.enderecoandarsala sala
					ON uorgend.easid=sala.easid
				LEFT JOIN
					siorg.enderecoandar andar
					ON sala.enaid=andar.enaid
				LEFT JOIN
					siorg.endereco endereco
					ON andar.endid=endereco.endid
				WHERE bem.benid='".$benid."'";

		return $this->pegaLinha($sql);


	}

	/**
	 * Recupera apenas valores dos campos relacionados ao bem (cadastro bem material)
	 * para carregamento dos dados nas outras telas de cadastro de bens
	 * @name pegarRegistroSimples
	 * @author Silas Matheus
	 * @access public
	 * @param int $benid - Código do bem
	 * @return array
	 */
	public function pegarRegistroSimples($benid){

		$benid = addslashes($benid);

		$sql = "SELECT
					*
				FROM
					$this->stNomeTabela ben
				LEFT JOIN
					sap.empenho emp
					ON emp.empid=ben.empid
				WHERE
					ben.benid=$benid";

		$return = $this->carregar($sql);

		return $return[0];
	}


	/**
     * Listar em grid os bens através dos filtros informados
     * @name listarBemPorFiltro
     * @author Alysson Rafael
     * @access public
     * @param int    $tesid        - Tipo de entrada/saída a ser pesquisado
     * @param string $bendtentrada - Data de entrada do bem a ser pesquisada
     * @param int    $bennumproc   - Número do processo do bem a ser pesquisado
     * @param string $bendtproc    - Data do processo do bem a ser pesquisada
     * @param string $filtroBenStatus - Status do bem (A - ativo, I - inativo)
     * @return void
     */
	public function listarBemPorFiltro($tesid,$bendtentrada,$bennumproc,$bendtproc, $filtroBenStatus){

		$where = " WHERE 1=1 ";
		if(!empty($tesid)){
			$tesid = addslashes($tesid);
			$where .= " AND bem.tesid='".$tesid."' ";
		}
		if(!empty($bendtentrada)){

			//formata a data
			$bendtentrada = formata_data_sql($bendtentrada);
			$bendtentrada = addslashes($bendtentrada);

			$where .= " AND bem.bendtentrada='".$bendtentrada."' ";
		}
		if(!empty($bennumproc)){

			$bennumproc = str_ireplace('.', '', $bennumproc);
			$bennumproc = str_ireplace('-', '', $bennumproc);
			$bennumproc = addslashes($bennumproc);

			$where .= " AND bem.bennumproc='".$bennumproc."' ";
		}
		if(!empty($bendtproc)){

			//formata a data
			$bendtproc = formata_data_sql($bendtproc);
			$bendtproc = addslashes($bendtproc);

			$where .= " AND bem.bendtproc='".$bendtproc."' ";
		}

		$sql = "SELECT
					'<center>' ||
						CASE WHEN
							(bem.benstatus = 'I')
						THEN
							'
							<a style=\"cursor:pointer;\" onclick=\"detalharBemEmAberto(\''||bem.benid||'\');\">
								<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
							</a>
							<a style=\"cursor:pointer;\" onclick=\"' ||
								CASE WHEN
									((SELECT COUNT(*) FROM sap.bensmaterial bmt WHERE bem.benid=bmt.benid) = 0)
								THEN
									'if(confirm(\'Deseja excluir o registro?\')){removerBem(\''||bem.benid||'\');}'
								ELSE
									'alert(\'Registro não pode ser excluído! Verifique se já existem materiais registrados para este bem.\');'
								END
							|| '\">
								<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
							</a>'
						ELSE
							'<a style=\"cursor:pointer;\" onclick=\"detalharBem(\''||bem.benid||'\');\">
								<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
							</a>'
						END
					|| ' </center>' as acao,
					tipo.tesdsc,
					'<span style=\"display:none;\">' || bem.bendtentrada || '</span>' || TO_CHAR(bem.bendtentrada, 'DD/MM/YYYY') AS bendtentrada,
					SUBSTR(cast(bem.bennumproc AS text) , 1 , 5)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 6 , 6)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 12 , 4)||'-'||SUBSTR(cast(bem.bennumproc AS text) , 16 , 2) AS bemnumproc,
					'<span style=\"display:none;\">' || bem.bendtproc || '</span>' || TO_CHAR(bem.bendtproc, 'DD/MM/YYYY') AS bendtproc,
                    (select sum(bmtitmat) from sap.bensmaterial selsoma where bem.benid=selsoma.benid) || ' ' AS benitdoc,
                    bem.benvlrdoc
                    FROM
                    	sap.bens bem
                    JOIN
                    	sap.tipoentradasaida tipo
                    	ON bem.tesid=tipo.tesid ";

		$sql .= $where." AND benstatus='$filtroBenStatus' ORDER BY bem.bendtentrada";

		$cabecalho = array("Comando","Tipo de Entrada","Data de Entrada","Número do Processo","Data do Processo","Qtd. de Ítens","Valor Total");
		$this->monta_lista($sql,$cabecalho,20,50,false,"center",'S','','',array('center','center','center','center','center','center','right'));

	}

	/**
     * Excluir um bem
     * @name excluirBem
     * @author Alysson Rafael
     * @access public
     * @param int $benid - Id do registro a ser excluído
     * @return void
     */
	public function excluirBem($benid, $link = "?modulo=principal/bens/processosAbertos&acao=A"){

		$benid = addslashes($benid);

		$sql = "DELETE FROM sap.bens WHERE benid='".$benid."'";
		$this->executar($sql);
		$this->commit();
        direcionar($link,'Operação realizada com sucesso!');

	}

	/**
     * Pega o valor do documento de acordo com o id do bem
     * @name pegarValorDocumento
     * @author Silas Matheus
     * @access public
     * @param int $benid - Id do Bem
     * @return float
     */
	public function pegarValorDocumento($benid){

		$benid = addslashes($benid);

		$sql = "SELECT
					benvlrdoc
				FROM
					$this->stNomeTabela
				WHERE
					benid=$benid";

		return $this->pegaUm($sql);

	}

	/**
     * Pega a quantidade de unidades total do documento de acordo com o id do bem
     * @name pegarTotalUnidadesDocumento
     * @author Silas Matheus
     * @access public
     * @param int $benid - Id do Bem
     * @return int
     */
	public function pegarTotalUnidadesDocumento($benid){

		$benid = addslashes($benid);

		$sql = "SELECT
					benitdoc
				FROM
					$this->stNomeTabela
				WHERE
					benid=$benid";

		return $this->pegaUm($sql);

	}

	/**
     * Soma os valores totais dos documentos por empenho
     * @name somarValoresTotaisDocumentos
     * @author Silas Matheus
     * @access public
     * @param int $empid - Id do Empenho
     * @return float
     */
	public function somarValoresTotaisDocumentos($empid){

		$empid = addslashes($empid);

		$sql = "SELECT
					SUM(benvlrdoc)
				FROM
					$this->stNomeTabela
				WHERE
					empid=$empid
				AND
					benstatus='A' ";

		return $this->pegaUm($sql);

	}


	/**
     * Carrega os registros para montar o cabeçalho do relatório
     * @name cabecalhoVisualizaEntradaBensCompra
     * @author Alysson Rafael
     * @access public
     * @param int $benid - Id do registro a ser pesquisado
     * @return array;
     */
	public function cabecalhoVisualizaEntradaBensCompra($benid){
		if(!empty($benid)){

			$benid = addslashes($benid);

			$sql = "SELECT 
					tipo.tesdsc,
					TO_CHAR(bem.bendtentrada, 'DD/MM/YYYY') AS bendtentrada,
					SUBSTR(cast(bem.bennumproc AS text) , 1 , 5)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 6 , 6)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 12 , 4)||'-'||SUBSTR(cast(bem.bennumproc AS text) , 16 , 2) AS bemnumproc,
					TO_CHAR(bem.bendtproc, 'DD/MM/YYYY') AS bendtproc,
					empenho.empnumero,
					TO_CHAR(empenho.empdata, 'DD/MM/YYYY') AS empdata,
					bem.bennumdoc,
					TO_CHAR(bem.bendtdoc, 'DD/MM/YYYY') AS bendtdoc,
					fornecedor.forrazaosocial,
					CASE 
						WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 14 THEN
							SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 2)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 3 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 6 , 3)||'/'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 9 , 4)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 13 , 2) 
						WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 11 THEN
					 		SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 4 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 7 , 3)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 10 , 2)
						ELSE CAST(fornecedor.forcpfcnpj AS text)
					END AS forcpfcnpj,
					bem.benobs
					
					FROM sap.bens bem 
					JOIN sap.tipoentradasaida tipo ON tipo.tesid=bem.tesid 
					LEFT JOIN sap.empenho empenho ON bem.empid=empenho.empid 
					LEFT JOIN sap.fornecedor fornecedor ON bem.forcpfcnpj=fornecedor.forcpfcnpj 
					WHERE bem.benid='{$benid}' ";

			$arDados = $this->pegaLinha($sql);
			$arDados = $arDados ? $arDados : array();
		}else{
			$arDados = array();
		}
		return $arDados;
	}
	
	/**
     * Carrega os registros para montar o cabeçalho do relatório
     * @name cabecalhoVisualizaEntradaBensDoacao
     * @author Alysson Rafael
     * @access public
     * @param int $benid - Id do registro a ser pesquisado
     * @return array;
     */
	public function cabecalhoVisualizaEntradaBensDoacao($benid){
		if(!empty($benid)){

			$benid = addslashes($benid);

			$sql = "SELECT 
					tipo.tesdsc,
					TO_CHAR(bem.bendtentrada, 'DD/MM/YYYY') AS bendtentrada,
					SUBSTR(cast(bem.bennumproc AS text) , 1 , 5)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 6 , 6)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 12 , 4)||'-'||SUBSTR(cast(bem.bennumproc AS text) , 16 , 2) AS bemnumproc,
					TO_CHAR(bem.bendtproc, 'DD/MM/YYYY') AS bendtproc,
					bem.dscdoc||' '||bem.bennumdoc AS documento,
					TO_CHAR(bem.bendtdoc,'DD/MM/YYYY') AS bendtdoc,
					CASE 
						WHEN CHAR_LENGTH(CAST(bem.bencnpjceddoa AS text)) = 14 THEN
							SUBSTR(CAST(bem.bencnpjceddoa AS text) , 1 , 2)||'.'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 3 , 3)||'.'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 6 , 3)||'/'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 9 , 4)||'-'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 13 , 2) 
						WHEN CHAR_LENGTH(CAST(bem.bencnpjceddoa AS text)) = 11 THEN
							SUBSTR(CAST(bem.bencnpjceddoa AS text) , 1 , 3)||'.'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 4 , 3)||'.'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 7 , 3)||'-'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 10 , 2) 
						ELSE CAST(bem.bencnpjceddoa AS text) 
					END AS bencnpjceddoa, 
					bem.benceddoa,
					bem.benobs
					FROM sap.bens bem 
					JOIN sap.tipoentradasaida tipo ON tipo.tesid=bem.tesid 
					WHERE bem.benid='{$benid}' ";

			$arDados = $this->pegaLinha($sql);
			$arDados = $arDados ? $arDados : array();
		}else{
			$arDados = array();
		}
		return $arDados;
		
	}
	
	/**
     * Carrega os registros para montar o cabeçalho do relatório
     * @name cabecalhoVisualizaEntradaBensSubstituicao
     * @author Alysson Rafael
     * @access public
     * @param int $benid - Id do registro a ser pesquisado
     * @return array;
     */
	public function cabecalhoVisualizaEntradaBensSubstituicao($benid){
		if(!empty($benid)){

			$benid = addslashes($benid);

			$sql = "SELECT 
					tipo.tesdsc,
					TO_CHAR(bem.bendtentrada, 'DD/MM/YYYY') AS bendtentrada,
					SUBSTR(cast(bem.bennumproc AS text) , 1 , 5)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 6 , 6)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 12 , 4)||'-'||SUBSTR(cast(bem.bennumproc AS text) , 16 , 2) AS bemnumproc,
					TO_CHAR(bem.bendtproc, 'DD/MM/YYYY') AS bendtproc,
					bem.bennumdoc,
					TO_CHAR(bem.bendtdoc,'DD/MM/YYYY') AS bendtdoc,
					CASE 
						WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 14 THEN 
							SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 2)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 3 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 6 , 3)||'/'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 9 , 4)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 13 , 2) 
						WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 11 THEN 
							SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 4 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 7 , 3)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 10 , 2) 
						ELSE CAST(fornecedor.forcpfcnpj AS text) 
					END AS forcpfcnpj, 
					fornecedor.forrazaosocial,
					bem.benobs,
					bem.ternumanterior
					FROM sap.bens bem 
					JOIN sap.tipoentradasaida tipo ON tipo.tesid=bem.tesid 
					LEFT JOIN sap.fornecedor fornecedor ON bem.forcpfcnpj=fornecedor.forcpfcnpj 
					LEFT JOIN sap.termo termo ON bem.ternum=termo.ternum 
					WHERE bem.benid='{$benid}' ";

			$arDados = $this->pegaLinha($sql);
			$arDados = $arDados ? $arDados : array();
		}else{
			$arDados = array();
		}
		return $arDados;
	}
	
	/**
     * Carrega os registros para montar o cabeçalho do termo
     * @name cabecalhoVisualizaTermoCompra
     * @author Alysson Rafael
     * @access public
     * @param int $benid - Id do registro a ser pesquisado
     * @return array;
     */
	public function cabecalhoVisualizaTermoCompra($benid){
		if(!empty($benid)){

			$benid = addslashes($benid);

			$sql = "SELECT tipo.tesdsc,";
			$sql .= "SUBSTR(cast(bem.bennumproc AS text) , 1 , 5)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 6 , 6)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 12 , 4)||'-'||SUBSTR(cast(bem.bennumproc AS text) , 16 , 2) AS bemnumproc,";
			$sql .= "TO_CHAR(bem.bendtproc, 'DD/MM/YYYY') AS bendtproc,TO_CHAR(bem.bendtentrada, 'DD/MM/YYYY') AS bendtentrada,";
			$sql .= "termo.ternum,TO_CHAR(termo.teremissaodt, 'DD/MM/YYYY') AS terdata,";
			$sql .= "empenho.empnumero,TO_CHAR(empenho.empdata, 'DD/MM/YYYY') AS empdata, ";
			$sql .= "bem.benobs,bem.bennumdoc,TO_CHAR(bem.bendtdoc,'DD/MM/YYYY') AS bendtdoc, ";
			$sql .= "CASE 
				         WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 14 THEN
				             SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 2)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 3 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 6 , 3)||'/'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 9 , 4)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 13 , 2) 
				         WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 11 THEN
					         SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 4 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 7 , 3)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 10 , 2)
				         ELSE CAST(fornecedor.forcpfcnpj AS text)
				     END AS forcpfcnpj, ";
			$sql .= "fornecedor.forrazaosocial,bem.ternumanterior ";
			$sql .= "FROM sap.bens bem ";
			$sql .= "JOIN sap.tipoentradasaida tipo ON tipo.tesid=bem.tesid ";
			$sql .= "LEFT JOIN sap.empenho empenho ON bem.empid=empenho.empid ";
			$sql .= "LEFT JOIN sap.fornecedor fornecedor ON bem.forcpfcnpj=fornecedor.forcpfcnpj ";
			$sql .= "LEFT JOIN sap.termo termo ON bem.ternum=termo.ternum ";
			$sql .= "WHERE bem.benid='".$benid."' ";

			$arDados = $this->pegaLinha($sql);
			$arDados = $arDados ? $arDados : array();
		}else{
			$arDados = array();
		}
		return $arDados;
	}

	/**
     * Carrega os registros para montar o cabeçalho do termo
     * @name cabecalhoVisualizaTermoDoacao
     * @author Alysson Rafael
     * @access public
     * @param int $benid - Id do registro a ser pesquisado
     * @return array;
     */
	public function cabecalhoVisualizaTermoDoacao($benid){
		if(!empty($benid)){

			$benid = addslashes($benid);
			
			$sql = "SELECT 
					tipo.tesdsc,
					TO_CHAR(bem.bendtentrada, 'DD/MM/YYYY') AS bendtentrada,
					termo.ternum,
					TO_CHAR(termo.teremissaodt, 'DD/MM/YYYY') AS terdata,
					SUBSTR(cast(bem.bennumproc AS text) , 1 , 5)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 6 , 6)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 12 , 4)||'-'||SUBSTR(cast(bem.bennumproc AS text) , 16 , 2) AS bemnumproc,
					TO_CHAR(bem.bendtproc, 'DD/MM/YYYY') AS bendtproc,
					bem.dscdoc||' '||bem.bennumdoc AS documento,
					TO_CHAR(bem.bendtdoc,'DD/MM/YYYY') AS bendtdoc,
					CASE 
						WHEN CHAR_LENGTH(CAST(bem.bencnpjceddoa AS text)) = 14 THEN
							SUBSTR(CAST(bem.bencnpjceddoa AS text) , 1 , 2)||'.'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 3 , 3)||'.'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 6 , 3)||'/'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 9 , 4)||'-'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 13 , 2) 
						WHEN CHAR_LENGTH(CAST(bem.bencnpjceddoa AS text)) = 11 THEN
							SUBSTR(CAST(bem.bencnpjceddoa AS text) , 1 , 3)||'.'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 4 , 3)||'.'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 7 , 3)||'-'||SUBSTR(CAST(bem.bencnpjceddoa AS text) , 10 , 2) 
						ELSE CAST(bem.bencnpjceddoa AS text) 
					END AS bencnpjceddoa, 
					bem.benceddoa,
					bem.benobs,
					bem.ternumanterior
					FROM sap.bens bem 
					JOIN sap.tipoentradasaida tipo ON tipo.tesid=bem.tesid 
					LEFT JOIN sap.termo termo ON bem.ternum=termo.ternum 
					WHERE bem.benid='{$benid}' ";
			
			$arDados = $this->pegaLinha($sql);
			$arDados = $arDados ? $arDados : array();
		}
		else{
			$arDados = array();
		}
		return $arDados;
	}
	
	/**
     * Carrega os registros para montar o cabeçalho do termo
     * @name cabecalhoVisualizaTermoSubstituicao
     * @author Alysson Rafael
     * @access public
     * @param int $benid - Id do registro a ser pesquisado
     * @return array;
     */
	public function cabecalhoVisualizaTermoSubstituicao($benid){
		if(!empty($benid)){

			$benid = addslashes($benid);
			
			$sql = "SELECT 
					tipo.tesdsc,
					TO_CHAR(bem.bendtentrada, 'DD/MM/YYYY') AS bendtentrada,
					termo.ternum,
					TO_CHAR(termo.teremissaodt, 'DD/MM/YYYY') AS terdata,
					SUBSTR(cast(bem.bennumproc AS text) , 1 , 5)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 6 , 6)||'.'||SUBSTR(cast(bem.bennumproc AS text) , 12 , 4)||'-'||SUBSTR(cast(bem.bennumproc AS text) , 16 , 2) AS bemnumproc,
					TO_CHAR(bem.bendtproc, 'DD/MM/YYYY') AS bendtproc,
					bem.bennumdoc,
					TO_CHAR(bem.bendtdoc,'DD/MM/YYYY') AS bendtdoc,
					CASE 
						WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 14 THEN 
							SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 2)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 3 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 6 , 3)||'/'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 9 , 4)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 13 , 2) 
						WHEN CHAR_LENGTH(CAST(fornecedor.forcpfcnpj AS text)) = 11 THEN 
							SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 1 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 4 , 3)||'.'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 7 , 3)||'-'||SUBSTR(CAST(fornecedor.forcpfcnpj AS text) , 10 , 2) 
						ELSE CAST(fornecedor.forcpfcnpj AS text) 
					END AS forcpfcnpj, 
					fornecedor.forrazaosocial,
					bem.benobs,
					bem.ternumanterior
					FROM sap.bens bem 
					JOIN sap.tipoentradasaida tipo ON tipo.tesid=bem.tesid 
					LEFT JOIN sap.fornecedor fornecedor ON bem.forcpfcnpj=fornecedor.forcpfcnpj 
					LEFT JOIN sap.termo termo ON bem.ternum=termo.ternum 
					WHERE bem.benid='{$benid}' ";
			
			$arDados = $this->pegaLinha($sql);
			$arDados = $arDados ? $arDados : array();
			
		}
		else{
			$arDados = array();
		}
		return $arDados;
	}


	/**
     * Carrega os registros para montar a lista do relatório
     * @name listaVisualizaEntradaBens
     * @author Alysson Rafael
     * @access public
     * @param int $benid - Id do registro a ser pesquisado
     * @return void
     */
	public function listaVisualizaEntradaBens($benid){
		$sql = array();
		if(!empty($benid)){

			$benid = addslashes($benid);

			$sql = "SELECT material.matcodigo||' ' AS matcodigo,material.matdsc,bemmaterial.bmtitmat||' ' AS bmtitmat, '<span style=\"color: rgb(0, 102, 204);float:right;\">' || to_char(bemmaterial.bmtvlrunitario, '9G999D99') || '</span>' bmtvlrunitario ,(bemmaterial.bmtvlrunitario*bemmaterial.bmtitmat) AS total,";
			$sql .= "'<span style=\"display:none;\">' || bemmaterial.bmtgarantia || '</span>' || TO_CHAR(bemmaterial.bmtgarantia, 'DD/MM/YYYY') AS bmtgarantia ";
			$sql .= "FROM sap.material material ";
			$sql .= "JOIN sap.bensmaterial bemmaterial ON material.matid=bemmaterial.matid ";
			$sql .= "WHERE bemmaterial.benid='".$benid."' ";
		}
		$cabecalho = array('C&oacute;digo','Descri&ccedil;&atilde;o do Material', 'Qtd. Entrada', 'Valor Unit&aacute;rio','Vlr. Total', 'Garantia');
		$this->monta_lista($sql,$cabecalho,9000,10,true,'center','S','','',array('center','center','center','right','right','center'));
	}
	
	
	/*public function listaVisualizaTermo($benid,$ccbid){
		$sql = array();
		if(!empty($benid) && !empty($ccbid)){

			$benid = addslashes($benid);
			$ccbid = addslashes($ccbid);

			$sql = "SELECT 
					rgp.rgpnum||' ' AS rgpnum,
					rgp.rgpnumserie||' ' AS rgpnumserie,
					bmt.bmtdscit,
					TO_CHAR(bmt.bmtgarantia,'DD/MM/YYYY') AS bmtgarantia,
					bmt.bmtvlrunitario
					FROM sap.rgp rgp 
					JOIN sap.bensmaterial bmt ON rgp.bmtid=bmt.bmtid 
					JOIN sap.bens bem ON bmt.benid=bem.benid 
					JOIN sap.material mat ON bmt.matid=mat.matid 
					JOIN sap.catalogo cat ON mat.catid=cat.catid
					JOIN sap.itemcontacontabil item ON cat.icbid=item.icbid AND cat.ccbid=item.ccbid
					JOIN sap.contacontabil conta ON item.ccbid=conta.ccbid
					WHERE bem.benid='{$benid}' 
					AND conta.ccbid='{$ccbid}'
					ORDER BY rgp.rgpnum ";
			
			return $this->carregar($sql);
			

		}
		//$cabecalho = array('RGP','N. de S&eacute;rie', 'Descri&ccedil;&atilde;o', 'Garantia','Valor');
		//$this->monta_lista($sql,$cabecalho,9000,10,true,'center','S','','',array('center','center','center','center','right'));
	}*/
	
	

	/**
	 * Verifica se registro já existe
	 * @name existe
	 * @author Silas Matheus
	 * @access public
	 * @param array $campos - Campos para verificar
	 * @return bool
	 */
	public function existe($campos){

		$campos['bennumdoc'] = addslashes($campos['bennumdoc']);

		$sql = "SELECT
					count(*) count
				FROM
					$this->stNomeTabela
				WHERE
					bennumdoc='{$campos['bennumdoc']}'
				AND
					benstatus='A'";

		if(!empty($campos['benid'])){
			$campos['benid'] = addslashes($campos['benid']);
			$sql .= " AND benid<>{$campos['benid']}";
		}

		$result = $this->carregar($sql);
 	   	$qtd = $result[0]['count'];

 	   	if($qtd > 0)
 	   		$resultado = true;

		return $resultado;

	}

	/**
	 * Ativa o Bem passando o status para "A"
	 * @name ativar
	 * @author Silas Matheus
	 * @access public
	 * @param int $benid - Identificador do bem
	 * @return bool
	 */
	public function ativar($benid){

		$benid = addslashes($benid);
		
		$bem = $this->pegarRegistroSimples($benid);

		$oTermo = new Termo();
		$ternum = $oTermo->gerarTermo('EN',$bem['ternum']);

		// atualizar o bem para o status ativo
		if(empty($bem['ternum'])){
			$sql = "UPDATE
					sap.bens
				SET
					benstatus='A',
					ternum=$ternum
				WHERE benid=". $benid;
		}
		else{
			$sql = "UPDATE
					sap.bens
				SET
					benstatus='A',
					ternum=$ternum,
					ternumanterior=".$bem['ternum']."
				WHERE benid=". $benid;
		}
		
		$this->executar($sql);

		// atualizar os rgps para o status normal
		$sql = "UPDATE
					sap.rgp
				SET
					sbmid='2'
				WHERE
					bmtid in (select bmtid from sap.bensmaterial where benid=$benid);";
		$this->executar($sql);

		$this->commit();

	}


	/**
	 * Carrega os números de processo dos bens cadastrados com o empenho em questão
	 * @name carregaBensPorEmpenho
	 * @author Alysson Rafael
	 * @access public
	 * @param int $empid - Identificador do empenho
	 * @return string
	 */
	public function carregaBensPorEmpenho($empid){

		$empid = addslashes($empid);

		$sql = "SELECT bem.bennumproc FROM sap.bens bem WHERE bem.empid='".$empid."' ";
		$resultado = $this->carregar($sql);
		$processos = '';
		if(is_array($resultado)){
			foreach($resultado as $key => $value){
				$processos .= $value['bennumproc'].',';
			}
			$pos = strrpos($processos, ',');
			if($pos !== false){
				$processos = substr($processos,0,$pos);
			}
		}
		return $processos;
	}




	public function processosAbertos(){

		$oBensBaixa = new BensBaixa();
		$oMovimentacao = new BensMovimentacao();

		$sql = "SELECT * FROM (";

		$sql .= "SELECT

					'<center>
						<a style=\"cursor:pointer;\" onclick=\"detalharBemEmAberto(\''||bem.benid||'\');\">
							<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
						</a>
						<a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Todos os dados serão perdidos. Confirma a exclusão do processo aberto de Entrada ?\')){removerBem(\''||bem.benid||'\');}\">
							<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
						</a>
					</center>' as acao,

		bem.bendtentrada AS data,'Entrada' AS tipo,bem.bennumproc||' ' AS numprocesso,CAST(bem.bendtproc AS text) AS dataprocesso,usu.usunome ";
		$sql .= "FROM sap.bens bem LEFT JOIN seguranca.usuario usu ON bem.usucpf=usu.usucpf ";
		$sql .= "WHERE benstatus='I' ";

		$sql .= " UNION ";

		$sql .= "SELECT

					'<center>
						<a style=\"cursor:pointer;\" onclick=\"detalharBensBaixa(\''||baixa.bebid||'\');\">
							<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
						</a>
						<a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Todos os dados serão perdidos. Confirma a exclusão do processo aberto de Desfazimento ?\')){removerBensBaixa(\''||baixa.bebid||'\');}\">
							<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
						</a>
					</center>' as acao,

		baixa.bebdata AS data,'Desfazimento' AS tipo,baixa.bebnumprocesso||' ' AS numprocesso,CAST(baixa.bebdataprocesso AS text) AS dataprocesso,usu.usunome ";
		$sql .= "FROM sap.bensbaixa baixa LEFT JOIN seguranca.usuario usu ON baixa.usucpf=usu.usucpf ";
		$sql .= "WHERE bebsituacao='I' ";

		$sql .= " UNION ";

		$sql .= "SELECT

					'<center>
						<a style=\"cursor:pointer;\" onclick=\"detalharMovimentacaoEmAberto(\''||mov.mvbid||'\',\''||mov.mvbtipo||'\');\">
							<img src=\"/imagens/editar_nome.gif \" border=0 title=\"Alterar\">
						</a>
						<a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Todos os dados serão perdidos. Confirma a exclusão do processo aberto de Movimentação ?\')){removerMovimentacao(\''||mov.mvbid||'\');}\">
							<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
						</a>
					</center>' as acao,

		mov.mvbdata AS data,'Movimentação' AS tipo,'' AS numprocesso,'' AS dataprocesso,usu.usunome ";
		$sql .= "FROM sap.movimentacaobens mov LEFT JOIN seguranca.usuario usu ON mov.usucpf=usu.usucpf ";
		$sql .= "WHERE mvbstatus='I' ";

		$sql .= ")AS tabela ORDER BY data DESC ";



		$result = $this->carregar($sql);
		return $result;
	}



	public function listaProcessosAbertos($dados){
		$cabecalho = array("Comando","Data de Criação","Tipo de Processo","Número do Processo","Data do Processo","Usuário Responsável");
		$this->monta_lista_array($dados,$cabecalho,20,50,false,"center");
	}



	public function ativarPopup($benid,$termoBaixa){

		$benid = addslashes($benid);
		
		$bem = $this->pegarRegistroSimples($benid);

		$oTermo = new Termo();
		$ternum = $oTermo->gerarTermo('EN',$bem['ternum']);

		// atualizar o bem para o status ativo
		if(empty($bem['ternum'])){
			$sql = "UPDATE
					sap.bens
				SET
					benstatus='A',
					ternum=$ternum,
					ternumbaixa=".$termoBaixa."
				WHERE benid=". $benid;
		}
		else{
			$sql = "UPDATE
					sap.bens
				SET
					benstatus='A',
					ternum=$ternum,
					ternumanterior=".$bem['ternum'].",
					ternumbaixa=".$termoBaixa."
				WHERE benid=". $benid;
		}
		
		$this->executar($sql);

		// atualizar os rgps para o status normal
		$sql = "UPDATE
					sap.rgp
				SET
					sbmid='2'
				WHERE
					bmtid in (select bmtid from sap.bensmaterial where benid=$benid);";
		$this->executar($sql);

		$this->commit();

		return $ternum;

	}
	
	
	
	public function pegaSiglaTipoEntradaSaida($benid){
		$sql = "SELECT tipoentsad.tesslg FROM sap.tipoentradasaida tipoentsad ";
		$sql .= "JOIN sap.bens bem ON tipoentsad.tesid=bem.tesid ";
		$sql .= "WHERE bem.benid='{$benid}' ";
		
		return $this->pegaUm($sql);
	}


	public function listaContaContabilBem($benid){
		$sql = "SELECT conta.ccbid,conta.ccbdsc
				FROM sap.contacontabil conta
				JOIN sap.itemcontacontabil item ON conta.ccbid=item.ccbid
				JOIN sap.catalogo cat ON item.icbid=cat.icbid AND item.ccbid=cat.ccbid
				JOIN sap.material mat ON cat.catid=mat.catid
				JOIN sap.bensmaterial bemmat ON mat.matid=bemmat.matid
				JOIN sap.bens bem ON bemmat.benid=bem.benid
				JOIN sap.rgp rgp ON bemmat.bmtid=rgp.bmtid
				WHERE bem.benid='{$benid}'
				GROUP BY conta.ccbid,conta.ccbdsc
				ORDER BY conta.ccbid ";
		
		return $this->carregar($sql);
		
	}

	public function pegaDadosVisualizaTermo($benid,$ccbid){
		$benid = addslashes($benid);
		$ccbid = addslashes($ccbid);
                $dataHojeSql = formata_data_sql(date("d/m/Y"));     
                
		$sql = "SELECT rgp.rgpnum
                            || ' '                                 AS rgpnum,
                            rgp.rgpnumserie
                            || ' '                                 AS rgpnumserie,
                            bmt.bmtdscit,
                            To_char(bmt.bmtgarantia, 'DD/MM/YYYY') AS bmtgarantia,
                            bmt.bmtvlrunitario,
                            x.vdepreciacaoacumulada AS valor_depreciado

                        FROM   sap.rgp rgp
                            inner join sap.fn_relatorio_depreciacao_mensal('{$dataHojeSql}') x
                            on rgp.rgpnum= x.rgpnum	
                            JOIN sap.bensmaterial bmt
                                ON rgp.bmtid = bmt.bmtid
                            JOIN sap.bens bem
                                ON bmt.benid = bem.benid
                            JOIN sap.material mat
                                ON bmt.matid = mat.matid
                            JOIN sap.catalogo cat
                                ON mat.catid = cat.catid
                            JOIN sap.itemcontacontabil item
                                ON cat.icbid = item.icbid
                                    AND cat.ccbid = item.ccbid
                            JOIN sap.contacontabil conta
                                ON item.ccbid = conta.ccbid
                        WHERE bem.benid='{$benid}' 
                                AND conta.ccbid='{$ccbid}'
                        ORDER  BY rgp.rgpnum   ";
                                //echo $sql;
		return $this->carregar($sql);
		
	}
	
	public function pegaTermoBaixaDaEntradaTipoSubstituicao($benid){
		$benid = addslashes($benid);
		
		$sql = "SELECT termo.ternum AS ternumbaixa,
				TO_CHAR(termo.teremissaodt,'DD/MM/YYYY') AS terdatabaixa
				FROM sap.termo
				JOIN $this->stNomeTabela bem ON termo.ternum=bem.ternumbaixa
				WHERE bem.benid='{$benid}' ";
		
		return $this->pegaLinha($sql);
	}


}