<?php

/**
 * Modelo de Anexo de Peca Publicitária
 * @author Alysson Rafael
 */
class Anexo extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.anexo';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('aneid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'ipaid' => null,
		'anetitulo' => null,
		'anedsc' => null,
		'anestatus' => null
	);
	
	
	/**
     * Listagem de dados
     * @name listar_por_filtro
     * @author Alysson Rafael
     * @param array $post - array com os dados do form
     * @access public
     * @return void
     */
	public function listar_por_filtro($post){
		
		$where = " WHERE substr(item.ipanumitempad, 0, 5) >= '2015' ";
		if(!empty($post['padnumero'])){
			$post['padnumero'] = str_replace('/','',pg_escape_string($post['padnumero']));
			$where .= " AND pad.padnumero = '{$post['padnumero']}' ";
		}
		if($post['anexo'] == '0' || $post['anexo'] == '1'){
			$post['anexo'] = pg_escape_string($post['anexo']);
			if($post['anexo'] == '0'){
				$where .= " AND (SELECT COUNT(*) AS qtd FROM publicidade.anexo anexo2 JOIN publicidade.arquivo arq2 ON anexo2.aneid=arq2.aneid WHERE anexo2.ipaid=item.ipaid AND arq2.arqstatus='A') = 0 ";
			}
			else if($post['anexo'] == '1'){
				$where .= " AND (SELECT COUNT(*) AS qtd FROM publicidade.anexo anexo2 JOIN publicidade.arquivo arq2 ON anexo2.aneid=arq2.aneid WHERE anexo2.ipaid=item.ipaid AND arq2.arqstatus='A') > 0 ";
			}
		}
		
		$sql = "SELECT 
				  '<center>
                  <a style=\"cursor:pointer;\" onclick=\"javascript:detalhar(\''||item.ipaid||'\');\">
                  <img src=\"/imagens/anexo.gif \" border=0 title=\"Anexar arquivo\">
                  </a>
                  </center>' AS acao,
                  
                  SUBSTR(CAST(pad.padnumero AS text),1,4)||'/'||SUBSTR(CAST(pad.padnumero AS text),5,3) AS padnumero,
                  item.ipaid AS ipaid,
                  item.ipanumitempad AS ipanumitempad,
                  fornecedor.fornome AS fornome,
                  item.ipadsc AS ipadsc,
                  anexo.anetitulo AS anetitulo
				  FROM publicidade.itenspad item
				  JOIN publicidade.pad pad ON item.padid=pad.padid
				  JOIN publicidade.fornecedor fornecedor ON item.forid=fornecedor.forid
				  LEFT JOIN $this->stNomeTabela anexo ON item.ipaid=anexo.ipaid
                  {$where} 
                  ORDER BY pad.padnumero,item.ipanumitempad ";
        $resultado = $this->carregar($sql);
        
        if(is_array($resultado) && count($resultado) >= 1){
        	$control = '';
        	foreach($resultado as $key => $value){
        		if($control != $value['ipaid']){
        			$control = $value['ipaid'];
        			
        			//pega o(s) prazo(s) de veiculação
        			$sqlAnexo = "SELECT 
        						 TO_CHAR(prazo.pveprazoinicio,'DD/MM/YYYY')||' a '||TO_CHAR(prazo.pveprazotermino,'DD/MM/YYYY') AS vigencia
        						 FROM publicidade.anexo anexo
        						 LEFT JOIN publicidade.prazoveiculacao prazo ON anexo.aneid=prazo.aneid 
        						 WHERE anexo.ipaid='{$value['ipaid']}' AND prazo.pvestatus='A' ";
        			$rsAnexo = $this->carregar($sqlAnexo);
        			if(is_array($rsAnexo) && count($rsAnexo) >= 1){
        				$prazos = '';
        				foreach($rsAnexo as $keyAnexo => $valueAnexo){
        					$prazos .= $valueAnexo['vigencia'].'<br />';
        				}
        				$resultado[$key]['prazoveiculacao'] = $prazos;

        			}else{
        				$resultado[$key]['prazoveiculacao'] = '';
        			}

        			
        			//pega o(s) prazo(s)
        			$sqlAnexo = "SELECT 
        						 CASE WHEN prazo.ppeprazo = 'S'
        						 	THEN 'Indeterminado'
        						 	ELSE TO_CHAR(prazo.ppeprazoinicio,'DD/MM/YYYY')||' a '||TO_CHAR(prazo.ppeprazotermino,'DD/MM/YYYY')
        						 END AS vigencia
        						 FROM publicidade.anexo anexo
        						 LEFT JOIN publicidade.prazopeca prazo ON anexo.aneid=prazo.aneid 
        						 WHERE anexo.ipaid='{$value['ipaid']}' AND prazo.ppestatus='A' ";
        			$rsAnexo = $this->carregar($sqlAnexo);
        			if(is_array($rsAnexo) && count($rsAnexo) >= 1){
        				$prazos = '';
        				foreach($rsAnexo as $keyAnexo => $valueAnexo){
        					$prazos .= $valueAnexo['vigencia'].'<br />';
        				}
        				$resultado[$key]['prazo'] = $prazos;
        			}else{
        				$resultado[$key]['prazo'] = '';
        			}
        			
        			//pega o(s) arquivo(s)
        			$sqlArquivo = "SELECT 
        						   arquivo.arqid,
        						   arq.arqnome AS arqnome,
        						   anexo.aneid
        						   FROM publicidade.anexo anexo
        						   LEFT JOIN publicidade.arquivo arquivo ON anexo.aneid=arquivo.aneid
        						   LEFT JOIN public.arquivo arq ON arquivo.arqid=arq.arqid
        						   WHERE anexo.ipaid='{$value['ipaid']}' AND arquivo.arqstatus='A' ";
        			$rsArquivo = $this->carregar($sqlArquivo);
        			if(is_array($rsArquivo) && count($rsArquivo) >= 1){
        				$arquivos = '';
        				foreach($rsArquivo as $keyArquivo => $valueArquivo){
        					$arquivos .= "<a href=\"javascript:baixar('".$valueArquivo['arqid']."','".$valueArquivo['aneid']."');\">".$valueArquivo['arqnome'].'</a><br />';
        				}
        				$resultado[$key]['arqnome'] = $arquivos;
        			}else{
        				$resultado[$key]['arqnome'] = '';
        			}
        			
        		}
        		unset($resultado[$key]['ipaid']);
        	}
        }
		
		$cabecalho = array('A&ccedil;&atilde;o','N&#186; PAD','N&#186; Item da PAD','Fornecedor','Descri&ccedil;&atilde;o','T&iacute;tulo da Pe&ccedil;a','Per&iacute;odo de Veicula&ccedil;&atilde;o','Autoriza&ccedil;&atilde;o de Exibi&ccedil;&atilde;o','Arquivo(s) Anexo(s)');
		$this->monta_lista_array($resultado,$cabecalho,10,50,false,"center");
	}
	
	
	/**
     * Cadastrar/editar 
     * @name salvar
     * @author Alysson Rafael
     * @access public
     * @param array $post - Dados do formulário
     * @return array
     */
	public function salvar($post){
		extract($post);
		
		$anetitulo = pg_escape_string($anetitulo);
		$anedsc = pg_escape_string($anedsc);
		$ipaid = pg_escape_string($ipaid);
		
		//caso o id esteja vazio, a operação será de insert
		//caso não esteja vazio, a operação será de update
		if(empty($aneid)){
			$sql = "INSERT INTO $this->stNomeTabela(ipaid,anetitulo,anedsc) ";
			$sql .= "VALUES('{$ipaid}','{$anetitulo}','{$anedsc}') ";
		}
		else if(!empty($aneid)){
			$aneid = pg_escape_string($aneid);
			$sql = "UPDATE $this->stNomeTabela SET ipaid='{$ipaid}',anetitulo='{$anetitulo}',anedsc='{$anedsc}' ";
			$sql .= "WHERE aneid='{$aneid}' ";
		}
		
		$sql .= "RETURNING aneid ";
		
       	$aneid = $this->pegaUm($sql);

       	if(!empty($aneid)){
       		$this->commit();
       		return array(0=>true,1=>$aneid,2=>$ipaid,3=>'cadAnexo');
       	}
       	else{
       		return array(0=>false,1=>$aneid,2=>$ipaid,3=>'cadAnexo');
       	}
		
	}
	
	
	/**
     * Carrega um anexo pela peça publicitária
     * @name carrega_registro_por_peca
     * @author Alysson Rafael
     * @access public
     * @param array $ipaid - Identificador da peça
     * @return array
     */
	public function carrega_registro_por_peca($ipaid){
		if(!empty($ipaid)){
			$ipaid = pg_escape_string($ipaid);
			$sql = "SELECT anexo.aneid,
						   anexo.anetitulo,
						   anexo.anedsc,
						   anexo.anestatus	
						   FROM $this->stNomeTabela anexo 
						   WHERE anexo.ipaid='{$ipaid}' ";
			return $this->pegaLinha($sql);
		}
	}
	

	
}