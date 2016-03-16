<?php

/**
 * Modelo de Arquivo
 * @author Alysson Rafael
 */
class ArquivoPeca extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'publicidade.arquivo';

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('arquivoid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'aneid' => null,
		'arqid' => null,
		'arqstatus' => null
	);
	
	
	/**
     * Pega a qtd de arquivos de um anexo
     * @name pega_qtd_arquivos
     * @author Alysson Rafael
     * @access public
     * @param int $aneid - Identificador do registro pai
     * @return int
     */
	public function pega_qtd_arquivos($aneid){
		if(!empty($aneid)){
			$aneid = pg_escape_string($aneid);
			$sql = "SELECT COUNT(*) AS qtd 
					FROM $this->stNomeTabela arqpeca
					JOIN public.arquivo arq ON arqpeca.arqid=arq.arqid 
					WHERE arqpeca.aneid='{$aneid}' AND arqpeca.arqstatus='A' ";
			$resultado = $this->pegaUm($sql);
			return $resultado;
		}
	}
	
	
	/**
     * Listagem dos dados
     * @name listar_por_anexo
     * @author Alysson Rafael
     * @param int $aneid - identificador do registro pai
     * @access public
     * @return void
     */
	public function listar_por_anexo($aneid){
		
		if(!empty($aneid)){
			$aneid = pg_escape_string($aneid);
			
			$sql = " SELECT 
					 '<center>
                  	  <a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Deseja excluir o registro?\')){ excluirArquivo(\''||arqpeca.arqid||'\'); }\">
                  	  <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir arquivo\">
                  	  </a>
                  	  </center>' AS acao, 
					 
					 '<a href=\"javascript:baixar(\''||arqpeca.arqid||'\');\">'||arq.arqnome||'</a>' AS arqnome
					 FROM $this->stNomeTabela arqpeca
					 JOIN public.arquivo arq ON arqpeca.arqid=arq.arqid
					 WHERE arqpeca.aneid='{$aneid}' AND arqpeca.arqstatus='A' ";
			
			$cabecalho = array('A&ccedil;&atilde;o','Arquivo(s)');
			$this->monta_lista($sql,$cabecalho,10,50,false,"center");

		}
	}
	
	/**
     * Upload de arquivo
     * @name salvar
     * @author Alysson Rafael
     * @param array $post  - dados do form
     * @param array $files - dados do arquivo para upload
     * @access public
     * @return bool
     */
	public function salvar($post,$files){
		if($files['arquivo']){
			$post['aneid'] = pg_escape_string($post['aneid']);
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$foto = new FilesSimec('arquivo', array('aneid'=>$post['aneid']), 'publicidade');
			return $foto->setUpload();
		}
	}
	
	/**
     * Exclui o arquivo físico e inativa nas tabelas 
     * @name inativar
     * @author Alysson Rafael
     * @param int $arqid  - Identificador do registro
     * @param int $aneid  - Identificador do registro pai
     * @access public
     * @return void
     */
	public function inativar($arqid,$aneid){
		if(!empty($arqid) && !empty($aneid)){
			$arqid = pg_escape_string($arqid);
			$aneid = pg_escape_string($aneid);
			
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$foto = new FilesSimec('arquivo', array('aneid'=>$aneid), 'publicidade');
			
			//exclui o arquivo do diretório
			$foto->excluiArquivoFisico($arqid);
			
			$sql = "UPDATE $this->stNomeTabela SET arqstatus='I' WHERE arqid='{$arqid}' ";
			$this->executar($sql);
			$sql = "UPDATE public.arquivo SET arqstatus='I' WHERE arqid='{$arqid}' ";
			$this->executar($sql);
			$this->commit();
			
		}
	}
	
	/**
     * Baixa um arquivo 
     * @name baixar
     * @author Alysson Rafael
     * @param int $arqid  - Identificador do registro
     * @param int $aneid  - Identificador do registro pai
     * @access public
     * @return void
     */
	public function baixar($arqid,$aneid){
		if(!empty($arqid) && !empty($aneid)){
			$arqid = pg_escape_string($arqid);
			$aneid = pg_escape_string($aneid);
			
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$foto = new FilesSimec('arquivo', array('aneid'=>$aneid), 'publicidade');
			
			//baixa o arquivo
			$foto->getDownloadArquivo($arqid);
		}
	}
	

	
}