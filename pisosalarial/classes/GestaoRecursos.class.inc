<?php
/**
 * Modelo de Gestão de Recursos
 * @author Wesley Romualdo da Silva
 */
class GestaoRecursos extends Modelo{

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pisosalarial.gestaorecurso";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "gerid" );

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'arqid' => null,
									  	'gerdsc' => null,
									  	'gerdata' => null,
									  	'gertipo' => null
									  );

	/**
     * Lista os documentos que foram anexados
     * @name listaDocumentoAnexados
     * @author Wesley Romualdo da Silva
     * @access public
     * @return void
     */
	public function listaDocumentoAnexados(){
	    $stWhere = "";
	    $stCampos = "";
	    if(!empty($_SESSION['piso']['muncod'])){
            $stWhere .= "WHERE g.muncod = '{$_SESSION['piso']['muncod']}'";
	    }
	    if(possuiPerfil(array(PERFIL_CADASTRO_MUNICIPAL))){
            $stCampos .= "<a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Deseja excluir o registro?\')){excluirAnexo(\''||to_char(g.gerdata, 'DD/MM/YYYY')||'\',\''||g.gerid||'\',\''||a.arqid||'\');}\">
                        <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\">
                        </a>";
	    }
	    //<a style=\"cursor:pointer;\" onclick=\"visualizaAnexo(\''||g.arqid||'\');\">
		$sql = "SELECT
					'<center>
		            {$stCampos}
                    <a style=\"cursor:pointer;\" onclick=\"javascript:download_file('||g.arqid||');\">
                    <img src=\"/imagens/consultar.gif\" border=0 title=\"Visualizar\">
                    </a>
                    </center>' AS acao,
				  g.gerdsc,
				  a.arqnome || '.' || a.arqextensao as arquivo,
				  to_char(g.gerdata, 'DD/MM/YYYY HH24:MI:SS') as data
				FROM pisosalarial.gestaorecurso g
				INNER JOIN arquivo a ON a.arqid = g.arqid
				{$stWhere}
				ORDER BY g.gerdata DESC";

		$cabecalho = array("Ações","Documentos","Nome do arquivo","Data do Anexo");
		$this->monta_lista($sql,$cabecalho,100,50,false,"center");
	}

	/**
     * Insere dados no banco
     * @name insereAnexos
     * @author Wesley Romualdo da Silva
     * @access public
	 * @param array $post - Dados do formulário
     * @return string
     */
	public function insereAnexos( $post ){

	    if(!empty($_SESSION['piso']['muncod'])){
            $stWhere = "and muncod = '{$_SESSION['piso']['muncod']}'";
        }

		$tipo = $post['tipoanexo'];
		$sql = "SELECT
		           gerid
		        FROM pisosalarial.gestaorecurso
		        where gertipo = '{$tipo}'
		        --and to_char(gerdata, 'YYYY-MM-DD') = '".date('Y-m-d')."'
		        {$stWhere}";
		$plcid = $this->pegaUm( $sql );
		if( empty( $plcid ) ){
			if( $tipo == 'LE' ){
				if( $_FILES["arquivolei"]["name"] ){
					$campos	= array("gertipo"  => "'LE'",
									"gerdsc" => "'Lei Municipal'"
									);

					$typeArquivo = $_FILES['arquivolei']['type'];
					unset($_FILES["arquivodecreto"]);
					unset($_FILES["arquivooutro"]);
				}
			} else if( $tipo == 'DE' ){
				if( $_FILES["arquivodecreto"]["name"] ){
					$campos	= array("gertipo"  => "'DE'",
									"gerdsc" => "'Decreto'"
									);

					$typeArquivo = $_FILES['arquivodecreto']['type'];
					unset($_FILES["arquivolei"]);
					unset($_FILES["arquivooutro"]);
				}
			} else if( $tipo == 'OU' ){
				if( $_FILES["arquivooutro"]["name"] ){
					$campos	= array("gertipo"  => "'OU'",
									"gerdsc" => "'Outros'"
									);

					$typeArquivo = $_FILES['arquivooutro']['type'];
					unset($_FILES["arquivodecreto"]);
					unset($_FILES["arquivolei"]);
				}
			}
			$resultado = 'naopdf';
			$campos['muncod'] = $_SESSION['piso']['muncod'];

			$arTipos = array('application/pdf',
			                 'application/msword',
			                 'application/vnd.oasis.opendocument.text',
			                 'application/x-vnd.oasis.opendocument.text',
			                 'application/vnd.openxmlformats-officedocument.wordprocessingml.document');

//			if( $typeArquivo == 'application/pdf'){
            if(in_array($typeArquivo, $arTipos)){
				if( is_array( $campos ) ){
					$file = new FilesSimec("gestaorecurso", $campos ,"pisosalarial");
					$arquivoSalvo = $file->setUpload();
					if( $arquivoSalvo ) $resultado = 'ok';
					else $resultado = 'naosalvo';
				} else {
					$resultado = 'naosalvo';
				}
			}
		} else {
			$resultado = 'existe';
		}
		return $resultado;
	}

	/**
     * Exclir registro pelo id da tabela
     * @name excluirAnexos
     * @author Wesley Romualdo da Silva
     * @access public
	 * @param int $arqidDel - Id do registro que será deletado
     * @return string
     */
	public function excluirAnexos( $arqidDel ){
		if($arqidDel){
		    $sql = "DELETE FROM pisosalarial.gestaorecurso where arqid=".$arqidDel;
		    $this->executar($sql);
		    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$arqidDel;
		    $this->executar($sql);
		    if($this->commit()){
			    $file = new FilesSimec();
				$file->excluiArquivoFisico($arqidDel);
				$resultado = 'ok';
		    } else {
		    	$resultado = 'erro';
		    }

			return $resultado;
		}
	}

	/**
     * Download do arquivo anexado
     * @name downloadAnexo
     * @author Wesley Romualdo da Silva
     * @access public
	 * @param int $arqid - Id do arquivo
     * @return string
     */
	public function downloadAnexo( $arqid ){
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($arqid);
	}
}
?>