<?php
include_once APPRAIZ . "includes/classes/file.class.inc";
/**
 * Class FilesSimec
 * @description : Classe manipuladora de Arquivos. Salva dos dados referente aos arquivos nas tabelas do bando de dados do SIMEC
 * @category file (manipuladora de arquivos exclusiva esquema SIMEC)
 * @version 1.0
 * @author Thiago Tasca Barbosa
 * @since 03/03/2009
 * @package: file.class.inc ( Classe Files() ).
 * @tutorial: O SIMEC trabalha da seguinte forma para gravação de arquivos pelo sistema:
 * 				- 1º Salva as informações dos arquivos na tabela public.arquivo
 * 				- 2º Salva a chave primaria (arqid) da tabela  public.arquivo na tabela do esquema desejado para referencia do arquivo.
 * 				- 3º Salva o arquivo no caminho informado, utilizando a classe file.class.inc
 * @example : include_once APPRAIZ . "includes/classes/file.class.inc";
 *			  include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
 * 			  $file = new FilesSimec("Tabela do Esquema", "Array de campos da tabela" ,"Esquema");
 */
class FilesSimec extends Files {
	
	private $db;
	private $nomeEsquema;
	private $idSistema;
	private $tabela;
	private $idArquivo;
	private $campos;
	private $campoRetorno;
	private $pulaTableEschema;
	
	/**
	 * function __construct 
	 * @description : Método construtor da classe.
	 * @param  : string $tabela
	 * @param  : array  $campos 
	 * @param  : string $nomeSistema
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial :
	 * 	Existe 3 parametros a serem passados:
	 * 	Sendo o 1º: Tabela do esquema.
	 * 			2º: Campos da tabela do esquema.
	 * 			3º: Nome do Sistema (não obrigatorio).
	 * 
	 * 	A função construtora carrega os dados minimos para que se possa gravar os dados no banco de dados.
	 *  E carregado: a conexão com o banco, 
	 * 				 o esquema, caso não se tenha passado o esquema a função entende que o esquema será o que a pessoa estiver no momento,
	 * 				 a tabela do esquema.
	 * 				 e os campos da tabela do esquema. 
	 * @example: include_once APPRAIZ . "includes/classes/file.class.inc";
	 *			 include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	 * 			 $file = new FilesSimec("convenioxml", $campos ,"cte");
	 */
	public function __construct($tabela = NULL, $campos = NULL, $esquema = NULL){
		
		$this->conexaoBanco(); 				// Carrega conexão com o Banco.
		
		if( $tabela && $campos  ){
			try {
				if(!$tabela){
					throw new Exception("Obrigatorio informar a Tabela do Esquema.");
				} else
				if(!$campos){
					throw new Exception("Obrigatorio informar os campos da Tabela do Esquema.");
				} else {
					if(!$this->setEsquema($esquema)){ 			// Carrega o Esquema.
						throw new Exception("Não foi possivel instanciar o Módulo - Esquema.");	
					}	
					$this->setTabela($tabela);			// Carrega a Tabela do Esquema.
					$this->setCampos($campos);			// Carrega os Campos da Tabela instanciados.
				}
			}catch (Exception $e) {
				$this->setErro("Exceção na iniciação da classe FilesSimec: ".  $e->getMessage());
				$this->getErroSimec();
			}
		}else{
			try{
				if(!$this->setEsquema($esquema)){ 			// Carrega o Esquema.
					throw new Exception("Não foi possivel instanciar o Módulo - Esquema.");
				}
			}catch (Exception $e) {
				$this->setErro("Exceção na iniciação da classe FilesSimec: ".  $e->getMessage());
				$this->getErroSimec();
			}	
		}
	}
	
	/**
	 * function conexaoBanco()
	 * @description : Abre conexão com o Banco de dados pela classe do SIMEC cls_banco().
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 */
	private function conexaoBanco(){
		global $db;		
		
		if( !is_object($db) ){
			$db = new cls_banco();
		}
		$this->db = $db;
	}
	
	/**
	 * function setEsquema($nomeSistema = NULL) 
	 * @description : Instancia o Esquema do Módulo. Classe privada
	 * @param  : string $nomeSistema
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial : Caso não seja passado o Nome do Esquema a função recupera pelo ID do sistema que está na session.
	 */
	private function setEsquema($nomeSistema = NULL){
			if($nomeSistema){
				$this->nomeEsquema 	= $nomeSistema;
			}else{
				$this->nomeEsquema 	= $_SESSION["sisdiretorio"];
			}
			$this->idSistema 	= $_SESSION["sisid"];
			if($this->nomeEsquema && $this->idSistema){
				return true;
			}else{
				return false;
			}
	}
	
	/**
	 * function setTabela($tabela = NULL) 
	 * @description : Instancia a tabela do Esquema setado. Classe privada
	 * @param  : string $tabela
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial : Carrega para variavel global da classe o nome da tabela.
	 */
	private function setTabela($tabela){
		$this->tabela = $tabela;
	}
	
	/**
	 * function setCampos($campos) 
	 * @description : Carrega os campos da tabela. Classe privada
	 * @param  : array $campos
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial : Carrega campos da tabela para variavel global da classe.
	 */
	private function setCampos($campos){
		$this->campos = $campos;
	}
	
	/**
	 * function setUpload 
	 * @description : Salva o arquivo e os dados no banco de dados.
	 * @param  : string $descricaoArquivo (Campo de descrição do arquivo OPcional).
	 * @param  : string $key (Compo opcional com a chave do aray $_FILES. Ex: $_FILES[$key] )
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial :
	 * 	Existe 1 parametros a ser passados:
	 * 	Sendo o 1º: A pasta a onde será salvo o arquivo.
	 * 	Caso deseja passar a descrição do arquivo informa a variavel $descricaoArquivo.
	 * 
	 * 	Lembrando que por default 
	 *  E carregado: a conexão com o banco, 
	 * 				 o esquema, caso não se tenha passado o esquema a função entende que o esquema será o que a pessoa estiver no momento,
	 * 				 a tabela do esquema.
	 * 				 e os campos da tabela do esquema. 
	 * @example: include_once APPRAIZ . "includes/classes/file.class.inc";
	 *			 include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	 * 			 $file = new FilesSimec("convenioxml", $campos ,"cte");
	 */
	public function setUpload( $descricaoArquivo = NULL, $key = NULL , $insereTabelaSchema = true, $campoRetorno = false){
		if( $key == NULL ){
			Files::setFile();// Carrega o arquivo do tipo FILE.		
		}else{
			Files::setFile( NULL, NULL, $key );// Carrega o arquivo do tipo FILE com chave especifica.		
		}
		$this->inserePublicArquivoDB($descricaoArquivo); // Insere na tabela padrão do SIMEC de arquivos. (public.arquivo)
		
		if($insereTabelaSchema){
			if( $this->idArquivo ){
				$this->campoRetorno = $campoRetorno;
				$this->insereTabelaEsquemaDB(); // Insere na tabela do esquema.
			}else{
				return false;
			}
		}
		$caminhoEarquivo = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($this->idArquivo/1000).'/'.$this->idArquivo;
		$this->criaPasta(); // caso a pasta não exista então cria a pasta.
		Files::setCaminho($caminhoEarquivo);
		Files::upload($caminhoEarquivo);
		if($this->getErroSimec()){ // Se ocorreu tudo com sucesso.
			$this->db->commit();
			return true;
		}else{
			return false;
		}
	}
	
	/**
	 * function setMover 
	 * @description : Move um arquivo de uma pasta para outra 
	 * 				  OBS:  No esquema do SIMEC.
	 * 						Não move de uma pasta de um sistema para outro.
	 * @param  : string $origem (Arquivo de origem).
	 * @param  : string $tipoExtencao (tipo de arquivo).
	 * @author : Thiago Tasca Barbosa
	 * @since  : 08/05/2009
	 * @tutorial :
	 * 
	 * @example: 	include_once APPRAIZ."includes/classes/fileSimec.class.inc";
	 * 				$campos		= array("cnvid"	=>726);	
	 * 				$file 		= new FilesSimec("convenioxml", $campos);
	 * 				$arquivo 	= $file->criaArquivo(APPRAIZ."arquivos/".$descricao.".xml",$xml);
	 * 				$file->setMover(APPRAIZ."arquivos/".$descricao.".xml", "text/xml");
	 */
	public function setMover($origem, $tipoExtencao, $insereTabelaSchema = true){
		Files::setFile($origem, $tipoExtencao); // Carrega o arquivo do tipo FILE.
		$this->inserePublicArquivoDB($descricaoArquivo); // Insere na tabela padrão do SIMEC de arquivos. (public.arquivo)
		if($insereTabelaSchema){
			$this->insereTabelaEsquemaDB(); // Insere na tabela do esquema.
		}
		$destino = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($this->idArquivo/1000).'/'.$this->idArquivo;
		Files::setCaminho($destino);
		$this->criaPasta(); // caso a pasta não exista então cria a pasta.
		Files::mover($origem);
		if($this->getErroSimec()){ // Se ocorreu tudo com sucesso.
			$this->db->commit();
			return true;
		}else{
			$this->erro = "Não foi possivel deletar o arquivo do BD.";
			$this->getErroSimec();
			return false;
		}
	}
	
	
	/**
	 * function inserePublicArquivoDB() 
	 * @description : Salva os dados do arquivo na tabela public.arquivo.
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial : Está função salva todos os dados do arquivo na tabela public.arquivo. (Padrão SIMEC)
	 * 	Existe 1 parametros a ser passados mas não e obrigatorio:
	 * 	Sendo: $descricaoArquivo = Caso tenha um campo no formulário informando a descrição do arquivo
	 * 							   ele deverá ser salvo na public.arquivo.
	 */
	private function inserePublicArquivoDB($descricaoArquivo = NULL){
		$arquivo = $this->arquivo;
		//Insere o registro do arquivo na tabela public.arquivo
		if($arquivo["size"] != ''){

            $arquivo["name"]  = simec_addslashes($arquivo["name"]);
            $descricaoArquivo = simec_addslashes($descricaoArquivo);

			$arquivosparts = explode(".", $arquivo["name"]);
			$arqextensao = end($arquivosparts);
			
			$sql = "INSERT INTO public.arquivo (arqnome,
												arqextensao,
												arqdescricao,
												arqtipo,
												arqtamanho,
												arqdata,
												arqhora,
												usucpf,
												sisid,
												arqstatus)
					VALUES( '".current(explode(".", $arquivo["name"]))."',
							'".$arqextensao."',
							'".$descricaoArquivo."',
							'".$arquivo["type"]."',
							'".$arquivo["size"]."',
							'".date('Y-m-d')."',
							'".date('H:i:s')."',
							'".$_SESSION["usucpf"]."',
							$this->idSistema,
							'A') 
					RETURNING arqid;";
			$arqid = $this->db->pegaUm($sql);
			$this->idArquivo = $arqid;
		}
	}
	
	/**
	 * function insereTabelaEsquemaDB()
	 * @description : Insere os dados na tabela do Esquema utilizando os campos da tabela passado.
	 * @package	: function setCampos();
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial :  Insere na tabela do esquema passado os dados desejado.
	 */
	private function insereTabelaEsquemaDB(){
		$campos 		= $this->campos; // Recupera os campos da tabela.
		$camposInsert 	= implode( ",",array_keys($campos)); // nome do campo.
		$valorInsert 	= implode( ",",$campos); // valor do campo.
		//$campos = $this->recuperaCamposTabela();
		//Insere o registro na tabela do esquema
		
		if( $this->campoRetorno )
		{
			$sql = "INSERT INTO ".$this->nomeEsquema.".".$this->tabela." (".$camposInsert.",arqid)
					VALUES(".$valorInsert.",".$this->idArquivo.") RETURNING ".$this->campoRetorno.";";
			$this->campoRetorno = $this->db->pegaUm($sql);
		}
		else
		{
			$sql = "INSERT INTO ".$this->nomeEsquema.".".$this->tabela." (".$camposInsert.",arqid)
					VALUES(".$valorInsert.",".$this->idArquivo.");";
			$this->db->executar($sql);
		}
	}
	
	/**
	 * function criaPasta() 
	 * @description : Cria pasta.
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial : Cria a pasta caso não exista.
	 * 	Sendo que o Simec trabalha com arquivos e pastas da seguinte forma:
	 * 		1º Os arquivos serão salvo na pasta do Esquema.
	 * 		2º O arquivo e salvo dentro de uma pasta criada da seguinte forma: 
	 * 			- o ID do arquivo que está na tabela public arquivo / por 10000. (Feito istó para melhor indexação dos arquivos).
	 */
	private function criaPasta(){
		
		if(!is_dir(APPRAIZ."arquivos/".$this->nomeEsquema)){
			mkdir(APPRAIZ."arquivos/".$this->nomeEsquema, 0777);
		}
		
		if(!is_dir(APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($this->idArquivo/1000))){
			mkdir(APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($this->idArquivo/1000), 0777);
		}
	}
	
	/**
	 * function getErroSimec()
	 * @description : Retorna erros do SIMEC e desfaz operações feitas no banco.
	 * @package : file.class.inc
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @example: Caso do retorno seja TRUE, quer dizer que não tem erros. Operação com Sucesso.
	 */
	private function getErroSimec(){
		if($this->erro){
			if($this->db){
				$this->db->rollback();
				$this->idArquivo = null;
			}
			$this->getErro();
			return false;
		}else{
			return true;
		}
	}
	
	/**
	 * function getIdArquivo() 
	 * @description : Retorna o arqid do arquivo
	 * @author : Juliano Meinen
	 * @since  : 23/03/2011
	 * @example: $arqid = $file->getIdArquivo();
	 *			 echo($arqid);
	 */
	public function getIdArquivo()
	{
		return $this->idArquivo;
	}
	
	/**
	 * function getCampoRetorno() 
	 * @description : Retorna o campo de retorno previamente informado
	 * @author : Felipe Carvalho
	 * @since  : 08/07/2011
	 * @example: $anxid = $file->getCampoRetorno();
	 *			 echo($anxid);
	 */
	public function getCampoRetorno()
	{
		return $this->campoRetorno;
	}
	
	/**
	 * function getDownloadArquivo($arqid) 
	 * @description : Faz o Download do arquivo..
	 * @param  : string $arqid (ID do arquivo).
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @example: $arquivo = $file->getDownloadArquivo('13398');
	 *			 echo($arquivo);
	 */
	public function getDownloadArquivo($arqid){
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arqid;
		$arquivo = $this->db->carregar($sql);
		if($arquivo){
			$arquivo = current($arquivo);
		}else{
			$this->erro = "Arquivo não encontrado.";
			$this->getErroSimec();
			return false;
		}
		$caminho = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($arqid/1000).'/'.$arqid;
		if(!Files::Download($caminho)){
			if ($this->nomeEsquema == 'obras2'){
				$caminho = APPRAIZ.'arquivos/obras/'.floor($arqid/1000).'/'.$arqid;
				if(!Files::Download($caminho)){
					$this->getErroSimec();
					return false;
				}
			}else{
				$this->getErroSimec();
				return false;
			}
		}
	    $filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
	    header( 'Content-type: '. $arquivo['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();		   
	}
	
	/**
	 * function getArquivo($arqid) 
	 * @description : recupera o arquivo..
	 * @param  : string $arqid (ID do arquivo).
	 * @author : Apoena Machado Cunha
	 * @since  : 14/12/2011
	 * @example: $arquivo = $file->getArquivo('13398');
	 *			 echo($arquivo);
	 */
	public function getArquivo($arqid){
		$sql ="SELECT arqid FROM public.arquivo WHERE arqid = ".$arqid;
		$arquivo = $this->db->carregar($sql);
		
		if ($arquivo) {
			$arquivo = current($arquivo);
		} else {		    
			$this->erro = "Arquivo não encontrado.";
			$this->getErroSimec();
			return false;
		}
		
		$caminho = APPRAIZ . 'arquivos/' .  $this->nomeEsquema . '/' . floor($arqid/1000) . '/' . $arqid;

		if(file_exists($caminho) ){
			return $caminho;
		}else{
		    return false;
		}
	}
	
	/**
	 * Retorna as colunas da tabela arquivo correspondente ao arquivo informado
	 * e tambem o caminhofisico
	 *
	 * @param int $arqid
	 * @return array os dados do arquivo
	 */
	public function getDadosArquivo($arqid)
	{
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arqid;
		$arquivo = $this->db->carregar($sql);
		if($arquivo){
			$arquivo = current($arquivo);
		}else{		    
			$this->erro = "Arquivo não encontrado.";
			$this->getErroSimec();
			return false;
		}
		$arquivo['caminhofisico'] = $this->getCaminhoFisicoArquivo($arquivo['arqid']);

		return $arquivo;
		
	}
	
 	/* function excluiArquivoFisico($arqid) 
	 * @description : Faz o Exclusão física do arquivo..
	 * @param  : string $arqid (ID do arquivo).
	 * @author : Gustavo Fernandes
	 * @since  : 14/09/2009
	 * @example: $file->excluiArquivoFisico('13398');
	 */
	public function excluiArquivoFisico($arqid){
		$caminho = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($arqid/1000).'/'.$arqid;
		if(Files::Download($caminho)){
			unlink($caminho);
		}
	}
	

/**
 * function recuperaCamposTabela()
 * @description : Recupera os campos da tabela no esquema, caso não passado.
 * @author : Thiago Tasca Barbosa
 * @since  : 09/03/2009
 * @tutorial : Recupera todos os campos da tabela do Esquema.
 */
	/*
	public function recuperaCamposTabela(){
		$sql = "SELECT a.attname AS campo
				FROM  	pg_catalog.pg_class c,
						pg_catalog.pg_namespace nf,
						pg_catalog.pg_type t,
						pg_catalog.pg_attribute a
				LEFT JOIN pg_catalog.pg_description d ON d.objsubid = a.attnum AND a.attrelid = d.objoid
				LEFT JOIN pg_catalog.pg_constraint con ON con.conrelid = a.attrelid AND con.conkey[1] = a.attnum
				LEFT JOIN pg_catalog.pg_class clf ON con.confrelid = clf.oid AND clf.relkind = 'r'
				LEFT JOIN pg_catalog.pg_namespace nfa ON nfa.oid = clf.relnamespace
				LEFT JOIN pg_catalog.pg_attribute af ON af.attrelid = con.confrelid AND af.attnum = con.confkey[1]
				LEFT JOIN pg_catalog.pg_attribute af2 ON af2.attrelid = con.confrelid AND (af2.attname = (substr(af.attname, 0, 4) || 'dsc') OR af2.attname = (substr(af.attname, 0, 4) || 'desc'))
				WHERE
				nf.nspname = '".$this->nomeEsquema."' AND
				c.relname = '".$this->tabela."' AND 
				a.attnum > 0 AND
				a.attrelid = c.oid AND
				a.atttypid = t.oid AND
				nf.oid = c.relnamespace";

		$campos = $this->db->carregar($sql);
		$this->campos = $campos;
	}	
	*/

	public function setRemoveUpload( $arqid ){
		if ( $this->pulaTableEschema != true ){
			$this->deleteTabelaEsquemaDB($arqid);
		}
		$this->deletePublicArquivoDB($arqid);
		$this->excluiArquivoFisico($arqid);
		
		$this->db->commit();
		
		return true;
	}

	public function setPasta($nomePasta = NULL){
    	if($nomePasta){
    		$this->nomePasta 	= $nomePasta;
    	}
	}
	public function setPulaTableEschema($pula = false){
    	$this->pulaTableEschema = $pula;
	}
	
	private function deletePublicArquivoDB($arqid){
		//deleta arquivo da PublicArquivo
		if($arqid){
			$sql = "DELETE FROM public.arquivo WHERE arqid = ".$arqid;
			$executado = $this->db->executar($sql);
			if($executado){
				return true;
			}else{
				$this->erro = "Não foi possivel deletar o arquivo do BD.";
				$this->getErroSimec();
				return false;
			}
		}
	}
	
	private function deleteTabelaEsquemaDB($arqid){
		//deleta arquivo da PublicArquivo
		if($arqid){
			$sql = "DELETE FROM ".$this->nomeEsquema.".".$this->tabela." WHERE arqid = ".$arqid;
			$executado = $this->db->executar($sql);
			if($executado){
				return true;
			}else{
				$this->erro = "Não foi possivel deletar o arquivo do BD.";
				$this->getErroSimec();
				return false;
			}
		}
	}
	
	/**
	 * function setStream
	 * @description : Grava o arquivo a partir de uma stream de bytes
	 * @param  : string $descricaoArquivo (Campo de descrição do arquivo opcional).
	 * @param  : string $stream (Stream de bytes com o arquivo a ser gravado).
	 * @param  : string $tipo (Tipo do arquivo a ser gravado).
	 * @param  : string $extensao (Extensão do arquivo a ser gravado).
	 * @param  : string $insereTabelaSchema (Diz se os dados deve ser inseridos na tabela do esquema).
	 * @author : Silas Matheus
	 * @since  : 11/03/2011
	 * @tutorial :
	 *
	 * @example: 	include_once APPRAIZ."includes/classes/fileSimec.class.inc";
	 * 				$imagemVisitante= $_SESSION['imagemVisitante'];
	 * 				$campos			= array("nu_matricula_siape"	=>1234);
	 * 				$file 			= new FilesSimec("visitantefoto", $campos);
	 * 				$file->setStream("Foto de visitante", $imagemVisitante);
	 */
	public function setStream( $descricaoArquivo = NULL, $stream, $tipo = "image/jpeg", $extensao = ".jpg", $insereTabelaSchema = true, $nomeArquivo = "" ){
		if ($nomeArquivo)
		{
			$this->arquivo["name"] = $nomeArquivo;
		}
		$this->arquivo["size"] = strlen($stream);
		$this->arquivo["type"] = $tipo;
		$this->arquivo["extencao"] = $extensao;
		$this->inserePublicArquivoDB($descricaoArquivo); // Insere na tabela padrão do SIMEC de arquivos. (public.arquivo)
		if( $insereTabelaSchema ){
			$this->insereTabelaEsquemaDB(); // Insere na tabela do esquema.
		}

		$caminhoEarquivo = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($this->idArquivo/1000).'/'.$this->idArquivo;
		$this->criaPasta(); // caso a pasta não exista então cria a pasta.
		Files::setCaminho($caminhoEarquivo);
		
		$file = $caminhoEarquivo;
		$fp = fopen($file, "w+");
		$escreve = fwrite($fp, $stream);
		fclose($fp);
		
		if($this->getErroSimec()){ // Se ocorreu tudo com sucesso.
			$this->db->commit();
			return $this->idArquivo;
		}else{
			return false;
		}
		
	}

	/**
	 * function existeArquivo($arqid)
	 * @description : Diz se o arquivo existe realmente.
	 * @param  : string $arqid (ID do arquivo).
	 * @author : Marcos Queiroga
	 * @since  : 13/05/2011
	 * @example: $arquivo = $file->existeArquivo('13398');
	 */
	public function existeArquivo($arqid){
		$caminho = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($arqid/1000).'/'.$arqid;
		return (Files::Download($caminho));
	}

	/**
	 * function existeArquivo($arqid)
	 * @description : Diz se o arquivo existe realmente.
	 * @param  : string $arqid (ID do arquivo).
	 * @author : Marcos Queiroga
	 * @since  : 13/05/2011
	 * @example: $arquivo = $file->existeArquivo('13398');
	 */
	public function getCaminhoFisicoArquivo($arqid){
		$caminho = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($arqid/1000).'/'.$arqid;
        if(file_exists($caminho)){
            return $caminho;
        }else{
            return false;
        }

	}
}
?>
