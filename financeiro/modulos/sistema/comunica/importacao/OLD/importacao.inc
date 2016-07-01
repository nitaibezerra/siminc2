<?php
/**
 * Include de funções relativas a Importação de arquivo
 * Contem as rotinas de transformação do arquivo em uma matriz.
 * Busca dados para tabela de apoio
 * @param unknown_type $array
 */
//abre o  outro banco de dados de importação, que contem dados bruto usados no sistema (separados do banco original devido a quantidadde de dados trabalhados)

//include "config.inc";
//include APPRAIZ . "includes/classes_simec.inc";
//include APPRAIZ . "includes/funcoes.inc";

//$BancoImportacao = pg_connect("host=".$GLOBALS["servidor_bd"]." port=".$GLOBALS["porta_bd"]." dbname=  user=".$GLOBALS["usuario_db"] ." password=".$GLOBALS["senha_bd"] ."");

$BancoImportacao = $db->link;
//Servidor de Dados para importação
//$ServidorImport = $GLOBALS["servidor_bd"];


define( 'SIAF_DIR_TXT_FILES_DESTINO', APPRAIZ . 'financeiro/arquivos/siafi/'.$ano_referencia.'/EXECUCAO'.$ano_referencia.'/');
define( 'SIAF_DIR_TXT_FILES', APPRAIZ . 'financeiro/arquivos/siafi/'.$ano_referencia.'/' );



//Implementação de PogProgressBar
require_once(APPRAIZ . "includes/PogProgressBar.php" ); 


function ImprimeStatus($objeto,$valor,$cor = "")
{
	
	
	//#FF0000 - vermelho (erro)
	//#0000FF - azul (aguardando)
	//#FFFF00 - amarelo (executando)
	//#336633 - Verde Concluido
	
	?><script>
	document.<?php print $objeto;?>.size ='<?php print (strlen($valor)+15); ?>';
	document.<?php print $objeto;?>.value ='<?php print $valor; ?>';
	document.<?php print $objeto;?>.style.color ='<?php print $cor; ?>';
	</script>
	<?php
}



//Matrix de Erros do Sistema
$ErroImport = array();
  $ErroImport[1] = "Erro de lay-out: Quando os arquivos de lay-out (Ref) não possuírem os campos necessários para o SIMEC";
  $ErroImport[2] = "Erro por falta do caractere final de arquivo: quando o arquivo texto não possui o caractere final de arquivo"; //ok
  $ErroImport[3] = "Inconsistência de dados: quando os arquivos estão inconsistentes, por exemplo, quando o saldo contábil referencia-se a uma nota de empenho que não existe no sistema.";//N\A
  $ErroImport[4] = "Erro de arquivo corrompido";//N\A
  $ErroImport[5] = "Arquivo já processado";
  $ErroImport[6] = "Registro já inserido";//N\A
  $ErroImport[7] = "Erro por falta de espaço em disco";//N\A
  $ErroImport[8] = "Erro de leitura no arquivo";//N\A
  $ErroImport[9] = "Erro de permissão na geração do arquivo de execução";//N\A
  $ErroImport[10] = "Erro de permissão de execução do arquivo no banco de dados";//N\A
  $ErroImport[11] = "Erro de banco de dados: inconsistência nos dados";//N\A
  $ErroImport[12] = "Erro de banco de dados: falha no script de execução";
  $ErroImport[13] = "Erro de banco de dados: erro de integridade (violação de chave estrangeira)";//N/A
  $ErroImport[14] = "Erro de banco de dados: falha de conexão com o banco";//N\A
  $ErroImport[15] = "Erro de banco de dados: violação de chave primária";//N\A
  $ErroImport[16] = "Arquivos do diretorio de importação, já foram atualizados ou inexistentes.";
  $ErroImport[17] = "Criação do Arquivo";

  $sucessoImport = "Importação Concluida com Sucesso";

function rksort( &$array )
{
	if ( is_array( $array ) == false )
	{
		return;
	}
	ksort( $array );
	foreach ( array_keys( $array ) as $key )
	{
		rksort( $array[$key] );
	}
}

/**
 * Remove os campos não utilizados
 */


define( 'SIAF_MAX_MONTH', 21 );

//Classe que geração de Array apartir do arquivo
//falta verificar se a estrutura bate com a cadastrado no bd e caracter verficador
class ImportacaoArquivo
{
	
	
	
	/**
	 * Estruturas que definem o conteúdo dos arquivos a serem importados.
	 *
	 * @var unknown_type
	 */
	protected $estrutura = array();
	
	/**
	 * Relaciona um campo definido em um arquivo do SIAF à um campo de uma
	 * tabela do sistema.
	 *
	 * @var unknown_type
	 */
	protected $traducao = array();
	
	/**
	 * Enter description here...
	 *
	 * @var cls_banco
	 */
	protected $db = null;
	
	public function __construct()
	{
		global $nome_arquivo;
		global $SIAF_DIR_TXT_FILES;
		global $db;
		$this->db = $db;
	}
	
	protected function carregarEstrutura( $tipo )
	{
		// verifica se estrutura já foi carregada
		if ( array_key_exists( $tipo, $this->estrutura ) )
		{
			return;
		}
		
		// carrega arquivo que define estrutura
		$ref = $this->pegarRef();
		
		// o indice de cada campo é o nome, que contém as subchaves:
		// inicio, tamanho, dividir e repeticoes
		$campos = array();
		
		// utilizado para indicar onde se inicia um campo na linha
		// variável incremental
		$inicio = 0;
		
		// percorre arquivo linha a linha
		// o arquivo é carregado todo para a memória, pois seu tamanho não é grande
		foreach ( $ref as $linha )
		{
			// pega definições da linha
			$linha = trim( $linha );
			
			preg_match( '/([^\s]+)[\s]{1,}[a-z]{1}[\s]{1}(.*)/i', $linha, $match );
			
			// define nome
			$nome = trim( $match[1] );
					
			
			// define tamanho e se campo possui casas decimais
			// para o caso de decimais o tamanho da parte fracionada é
			// adicionada ao tamanho original do registro
			$casas_decimais = 0;
			if ( strpos( $match[2], ',' ) !== false )
			{
				$valores = explode( ',', $match[2] );
				$casas_decimais = $valores[1];
				$match[2] = $valores[0] + $valores[1];
			}
			$tamanho = (integer) $match[2];
			
			// define se é preciso realizar operações com o valor
			// ocorre nos casos de campos que possuem casas decimais
			$dividir = pow( 10, $casas_decimais );
			if ( $dividir == 1 )
			{
				$dividir = null;
			}
			
			// armazena definição da linha
			$campos[$nome] = array(
				'inicio' => $inicio,
				'tamanho' => $tamanho,
				'dividir' => $dividir,
				'repeticoes' => 1
			);
			
			// incrementa ponteiro de posição para a leitura de cada campo
			$inicio += $tamanho;
		}
		
		
		
		// monta estrutura final que define os dados presentes nos arquivos do tipo
		$this->estrutura[$tipo] = array(
			'campos' => $campos,
			// tamanho de cada linha no arquivo
			// ao final da leitura o a variavel aponta para o final da linha
			'tamanho_registro' => $inicio
		);
		
		
	}

	/**
	 * Importa dados de um arquivo SIOF
	 *
	 * ...
	 *
	 * @param string $arquivo

	 
	 */
	public function importarArquivo( $arquivo )
	{
		
		$GLOBALS["nome_arquivo"] = $arquivo;
		global $SIAF_DIR_TXT_FILES;
			// define caminho para o arquivo
		$caminho = $SIAF_DIR_TXT_FILES.$arquivo;
		
		
		// verifica se arquivo existe
		if ( file_exists( $caminho ) == false )
		{
			return;
		}
		
		//erro!
		// captura sigla/abreviação
		//preg_match( '/[^_]+_(.*)_[0-9]{8}\.txt/', $arquivo, $match );
		//$tipo = $match[1];
		
		switch (substr($arquivo,0,1))
		{
			case 's':
				$tipo = 'Saldo_Contabil';
				break;
			case 'n':
				$tipo = 'Nota_Empenho';
				break;
			case 'c':
				$tipo = 'Cambio';
				break;
			case 'p':
				switch(substr($arquivo,0,2))
				{
					case 'pc':
						$tipo = 'PC';
						break;
					case 'pt':
						$tipo = 'PTRES';
						break;
					default:
						$tipo = 'outros';
						break;
				}
				break;
			case 'u':
				$tipo = 'UG';
				break;
			case 'f':
				$tipo = 'FR';
				break;
			default:
				$tipo = 'outros';
				break;
		}
		// carrega estrutura para realizar importação
		$estrutura = array();
		$estrutura = $this->pegarEstrutura( $tipo );
		//dbg($estrutura);
		
		/**
		 * filtra os campos de acordo com a tabela de referência - inicio
		 */
		
		
		//$estrutura = array();
		
		//$estrutura_comp = $this->pegarEstrutura( $tipo );
		/*
		if ($tipo == 'Saldo_Contabil')
		{
			$sql="SELECT
					importacao.padraoarquivoregistro.prtid, 
					importacao.padraoarquivoregistro.prtdsc, 
					importacao.padraoarquivoregistro.parid, 
					importacao.padraoarquivoregistro.trrid, 
					importacao.padraoarquivoref.pararquivonome 
					FROM 
					importacao.padraoarquivoregistro 
					Inner Join importacao.padraoarquivoref ON importacao.padraoarquivoregistro.parid = importacao.padraoarquivoref.parid 
					WHERE 
					importacao.padraoarquivoref.pararquivonome =  '".$tipo."'";
				
			$dadosref = $this->db->carregar($sql);
			
			
			foreach ($dadosref as $ref)
			{
				if(!is_null($estrutura_comp['campos'][$ref['prtdsc']]))
				{
					$estrutura['campos'][$ref['prtdsc']]=$estrutura_comp['campos'][$ref['prtdsc']];
				}
			}
			
			$estrutura['tamanho_registro']= $estrutura_comp['tamanho_registro'];
		}else 
		{	
			$estrutura = $estrutura_comp;	
		}*/
		/**
		 * -Fim
		 */
		//dbg($estrutura,1);
		
		// para a quebra linha utilizam \r\n
		$tamanhoQuebraLinha = 2;
		
		// tamanho de cada linha
		$tamanho = $estrutura['tamanho_registro'] + $tamanhoQuebraLinha;
		
		// variáveis utilizadas para fins estatísticos relativo ao arquivo
		$totalRegistros = 0;
		$totalRegistrosRepetidos = 0;
		$registrofinal = array();
		
		// lê arquivo linha a linha
		$handle = fopen( $SIAF_DIR_TXT_FILES . $arquivo, 'r' );
		while( !feof( $handle ) )
		{
			$totalRegistros++;
			// variável utilizada para fins estatísticos relativos à linha
			$totalRegistrosLinha = 0;
			
			// lê registro completo
			$registroBruto = fread( $handle, $tamanho );
			if ( strlen( $registroBruto ) != $tamanho )
			{
				// TODO indicar erro de linha incorreta
				continue;
			}
			
			// remove quebra de linha ao final do registro
			$registroBruto = substr( $registroBruto, 0, -$tamanhoQuebraLinha );

			// um registro bruto pode conter vários registros
			
			// armazena campos que não se repetem
			$camposAgrupadores = array();
			
			// armazena os registros finais
			$registros = array();
			
			// percorre o conteúdo da linha campo a campo
			// a leitura é realizada de acordo com as definições da estrutura
			foreach ( $estrutura['campos'] as $nomeCampo => $dadosCampo )
			{
				// os campos que não se repetem são armazenados na lista de agrupadores
				if ( $dadosCampo['repeticoes'] == 1 )
				{
					$camposAgrupadores[$nomeCampo] = $dado = substr( $registroBruto, $dadosCampo['inicio'],  $dadosCampo['tamanho'] );
				}
				// os campos que se repetem são inseridos direto na lista de registros
				else
				{
					// caso o registro se repita
					for ( $mes = 1; $mes <= $dadosCampo['repeticoes']; $mes++ )
					{
						if ( $mes > SIAF_MAX_MONTH )
						{
							continue;
						}
						// cria o registro caso ele não exista
						if ( array_key_exists( $mes, $registros ) == false )
						{
							$registros[$mes] = array();
						}
						
						$camposAgrupadores[$nomeCampo] = $camposAgrupadores[$nomeCampo].substr( $registroBruto, $dadosCampo['inicio'],  $dadosCampo['tamanho'] );
					}
				}
			}
			
			
			$registros = $camposAgrupadores;
			$registrofinal[$totalRegistros] = $registros;
			
		}
		
		
		return $registrofinal;
		fclose( $handle );
		
	}
	
	protected function importarRegistro( $registro, $tipo )
	{
		// pegar informações que trduzem para onde os dados devem ir
		var_dump( $tipo );
		var_dump( $registro,1 );
	}
	
	/**
	 * Captura um arquivo .ref para um determinado tipo.
	 *
	 * O arquivo .ref carregado é o primeiro encontrado. Caso não exista um
	 * arquivo para o tipo determinado ou o tipo seja 'Saldo_Contabil' um texto
	 * vazio é retornado.
	 *  
	 * @return string[]
	 * 
	 * Função alterada 11/10/2006 por Alexandre
	 * 
	 * Agora a função retorna o arquivo de referencia igual ao nome
	 * do arquivo solicitado e uma array vazia caso o tipo não tiver
	 * sido solicitado
	 * 
	 */
	protected function pegarRef()
	{
		global $SIAF_DIR_TXT_FILES;
		$arquivos = glob($SIAF_DIR_TXT_FILES .''. str_replace('.txt','.ref',$GLOBALS["nome_arquivo"]));
		return count( $arquivos ) == 0 ? '' : file( current( $arquivos ) );
	}
	
	public function pegarEstrutura( $tipo )
	{
		$this->carregarEstrutura( $tipo );
		return $this->estrutura[$tipo];
	}
	
	public function pegarCampos( $tipo )
	{
		$estrutura = $this->pegarEstrutura( $tipo );
		return $estrutura['campos'];
	}
	
	
	
}


/**
 * Usado quando o campo a ser importado é do tipo redundate
 * Especifica o tipo de dado que este campo representa,
 * se é cnpj, cpf, ug, ig ou 999
 *
 * @param string $dado
 */
function IndentificaTipoCampo($dado)
{
	
	if ((strlen((trim($dado)))==14) and (substr($dado,11) != '000'))
	{
		return 'cnpj';
	}
	
	if (((strlen((trim($dado)))==14) and (substr($dado,11) == '000'))or(strlen((trim($dado)))==11))
	{
		return 'cpf';
	}
	
	return 'geral';
		
}

/**
 * Formata uma determinada string para formato string ou numerico (com ou sem aspas)
 * Coloca Null caso o valor a ser gravado for vazio
 *
 */
function Format_String($valor,$tipo)
{
	if (trim($valor)==''){ return 'NULL';}
	switch ($tipo)
	{
		case 'N': return str_replace("'","",$valor);
		case 'A': return "'".trim($valor)."'";
		default: return 'NULL';
	}
}

/**
 * Função retorna o campo o prefixo dos campos de acordo com a chave da tabela
 * usado para tratamento em tabelas de apoio
 */
function IndentificaPrefixo($campo)
{
	switch (substr($campo,strlen($campo)-2)){
		case 'id':	return substr($campo,0,strlen($campo)-2);
		case 'od':	return substr($campo,0,strlen($campo)-3);
		default: return '';
	}
}

/**
 * Busca o nome dos campos chave de uma tabela
 *
 * @param unknown_type $table
 * @param unknown_type $schema
 * @return unknown
 */
$Otimizacao_2 = array();
function pegarChave( $table, $schema = 'public' )
{
	
			global $Otimizacao_2;
			if (is_null($Otimizacao_2[$schema.$table]))
			{
				global $db;
				$table = str_replace( "'", "\\'", $table );
				$schema = str_replace( "'", "\\'", $schema );
				//$sql = "select column_name from information_schema.columns where table_schema = '" . $schema . "' and table_name = '" . $table . " and is_nullable = 'NO' order by ordinal_position;" ;
				
				$sql = "SELECT  
	    				--T.TABLE_NAME,  
	    				--T.CONSTRAINT_NAME,  
	    				K.COLUMN_NAME as coluna 
	    				--,K.ORDINAL_POSITION  
						FROM  
	    				INFORMATION_SCHEMA.TABLE_CONSTRAINTS T 
	    				INNER JOIN 
	    				INFORMATION_SCHEMA.KEY_COLUMN_USAGE K 
	    				ON T.CONSTRAINT_NAME = K.CONSTRAINT_NAME  
						WHERE 
	    				T.CONSTRAINT_TYPE = 'PRIMARY KEY'  
	    				AND T.TABLE_NAME = '" . $table . "' 
	    				AND T.table_schema = '" . $schema . "' 
						ORDER BY 
	    				T.TABLE_NAME, 
	    				K.ORDINAL_POSITION";
				
				
				$linhas = $db->carregar( $sql );
				if ( !$linhas )
				{
					return array();
				}
				$colunas = array();
				foreach ( $linhas as $linha )
				{
					array_push( $colunas, $linha['coluna'] );
				}
				
				$Otimizacao_2[$schema.$table] = $colunas;
				return $colunas;
			}else 
			{			
				return $Otimizacao_2[$schema.$table];
			}
			
			
			
}




/**
 * Busca se o dado esta a ser agravado esta na tabela de apoio 
 * e retorna o valor a ser gravado e a string de inserção na tabela de apoio (se for o caso)
 *Caso for uma tabela de chave dupla o valor de procura na variavel $campo é ignorado naquestao de procura
 * , procurando na chave primaria o valor necessario
 * @param unknown_type $tabela
 * @param unknown_type $campo
 * @param unknown_type $valor
 */


$MatrizApoio = array();
$tbapoio = array();
function Busca_Apoio($tabela,$campo,$valor)
{ 
	global $MatrizApoio;
	global $db;
	global $ano_referencia;
	
	

	if (($valor == 'NULL') || (str_replace(" ","",$valor) == "''") || (is_null($valor)))
	{
		$retorno["chave"] = null;
		$retorno["script"]="";	
		return $retorno;
	}
	//Caso os dados já formam buscados, retorna os valores armazenados na Matriz de apoio
	
	if (!is_null($MatrizApoio[$tabela][$campo][$valor]))
	{
		$retorno["chave"] = $MatrizApoio[$tabela][$campo][$valor];
		$retorno["script"]="";	
		return $retorno;
	}

	
	$retorno = array();
	$script_apoio = "";
	$cc = "";
	
	//faz tratamento de busca de chave dupla
	switch ($tabela)
	{
		case "fonterecursosiafi":
			$sql3 = "Select ''''||frscod||''','''||frsgrcod||'''' as cc from ".$tabela." Where frscod = ".substr($valor,0,3)."' and frsgrcod = '".substr($valor,3);
			$cc = "'cc'";
			break;
		case "orgao":
			$sql3 = "Select ''''||orgcod||''','''||organo||'''' as cc  from ".$tabela." Where orgcod = ".$valor." and organo = '".$ano_referencia."'";
			$cc = "'cc'";
			break;
		case "financeiro.planoconta":
			$sql3 = "Select ''''||concod||''','''||conano||'''' as cc from ".$tabela." Where concod = ".$valor." and conano = '".$ano_referencia."'";
			$cc = "'cc'";
			break;
		case "financeiro.ptres":
			$sql3 = "Select ''''||ptrcod||''','''||ptrano||'''' as cc from ".$tabela." Where ptrcod = ".$valor." and ptrano = '".$ano_referencia."'";
			$cc = "'cc'";
			break;
		case "naturezadespesa":
			$sql3 = "Select ndpid as cc from ".$tabela." Where ndpcod = ".$valor." and ndpano = '".$ano_referencia."'";
			$cc = "'cc'";
			break;
		
		default:
			$exp = explode('.',$tabela);
			$chaveb = array();
			if (is_null($exp[1]))
			{
				$chaveb = pegarChave($exp[0]);
			}else 
			{
				$chaveb = pegarChave($exp[1],$exp[0]);
			}
			$sql3 = "Select ".$chaveb[0]." from ".$tabela." Where ".$campo." = ".$valor;
			$cc = "'".$chaveb[0]."'";
			
			
			break;
			
	}
	//retorna o nome das colunas na tabela, para depois procurar o prefixo usado na tabela
	
	
	
	$tbval = $db->pegaUm($sql3,0);
	
	

	
	
	//pega o primeiro elemento (a chave da tabela)
	if (($tbval == false) && ($tbval != '0'))
	{
		$DadosIndireta_2 = '';
	}else 
	{
		
		$DadosIndireta_2 = $tbval;
			
		
	}
	
		
	//if ($tbval==false)
	
					
	/**
	* Caso não encontrado na tabela de apoio inserir o registro e coloca o proximo valor nextval
	*/
	
	
	$teste_Z = 'teste_'.$tbval; // tratamento 0 diferente de false
	
	if ($teste_Z=='teste_')
	{
				
		$esquema = explode('.',$tabela);
					
		if (is_null($esquema[1]))
		{
			
			
			$nomecolunas = pegarChave($tabela);
			$nomecolunas2 = $db->pegarColunas($tabela);
		}
		else 
		{
			$nomecolunas = pegarChave($esquema[1],$esquema[0]);
			$nomecolunas2 = $db->pegarColunas($esquema[1],$esquema[0]);
		}	
		
		
		
		$messagem = '';	
		$campodescricao = '';
		//trata excesões das tabela informação pessoal e chave duplas
		switch ($tabela)
		{
			//dados de indentificação pessoal
			case "identificacaopessoal":
				$data = getdate();
				$messagem = "'Importacao-".$data[mday]."-".$data[mon]."-".$data[year]."',";
				$chaveseq=$db->pegaUm("select nextval('".$tabela."_".$nomecolunas[0]."_seq')");
				$script_apoio = "Insert into ".$tabela."(".$nomecolunas[0].",".$campo.", idpnome,".IndentificaPrefixo($nomecolunas[0])."status) VALUES (".$chaveseq.",".trim($valor).",".$messagem."'N');\n";
				$dadoapoio = $chaveseq;
				$MatrizApoio[$tabela][$campo][$valor]=$dadoapoio;
				break;
			
	 		// dados de chave dupla public.fonterecursosiafi PRIMARY KEY (frscod char(2), frsgrcod varchar(6))
			case "fonterecursosiafi":
				$data = getdate();
				$messagem = "'Importacao-".$data[mday]."-".$data[mon]."-".$data[year]."',";
				$script_apoio = "Insert into ".$tabela."(frscod,frsgrcod,frsdsc,frsstatus) VALUES (".substr($valor,0,3)."','".substr($valor,3).",".$messagem."'N');\n";
				$dadoapoio = substr($valor,0,3)."','".substr($valor,3);
				$MatrizApoio[$tabela][$campo][$valor]=$dadoapoio;
				break;
			
			//dados de chave dupla public.orgao  PRIMARY KEY (orgcod char(5), organo char(4))	
			case "orgao":
				$data = getdate();
				$messagem = "'Importacao-".$data[mday]."-".$data[mon]."-".$data[year]."',";
				$script_apoio = "Insert into ".$tabela."(orgcod,organo,orgdsc,orgstatus) VALUES (".$valor.",'".$ano_referencia."',".$messagem."'N');\n";
				$dadoapoio = substr($valor,0,6)."','".$ano_referencia."'";
				$MatrizApoio[$tabela][$campo][$valor]=$dadoapoio;
				break;
			
			//dados de chave dupla financeiro.planoconta PRIMARY KEY (concod char(9), conano char(4))	
			case "financeiro.planoconta":
				$data = getdate();
				$messagem = "'Importacao-".$data[mday]."-".$data[mon]."-".$data[year]."',";
				$script_apoio = "Insert into ".$tabela."(concod,conano,condsc,constatus) VALUES (".$valor.",'".$ano_referencia."',".$messagem."'N');\n";
				$dadoapoio = substr($valor,0,10)."','".$ano_referencia."'";
				$MatrizApoio[$tabela][$campo][$valor]=$dadoapoio;
				break;
	 	 
			//dados de chave dupla financeiro.ptres PRIMARY KEY (ptrcod char(6), ptrano char(4))
			case "financeiro.ptres":
				$data = getdate();
				//$messagem = "'Importacao-".$data[mday]."-".$data[mon]."-".$data[year]."',";
				$script_apoio = "Insert into ".$tabela."(ptrcod,ptrano,ptrstatus) VALUES (".$valor.",'".$ano_referencia."','N');\n";
				$dadoapoio = substr($valor,0,7)."','".$ano_referencia."'";
				$MatrizApoio[$tabela][$campo][$valor]=$dadoapoio;
				break;	
			
			case "naturezadespesa":
				$data = getdate();
				$messagem = "'Importacao-".$data[mday]."-".$data[mon]."-".$data[year]."',";
				$chaveseq=$db->pegaUm("select nextval('".$tabela."_".$nomecolunas[0]."_seq')");
				$gndcod = Busca_Apoio("gnd","gndcod",substr($valor,2,1));
				if (($gndcod['chave'] == '') || is_null($gndcod['chave']))$gndcod['chave'] = 0;
				$script_apoio = $gndcod['script']."\n Insert into ".$tabela."(ndpid,gndcod,ndpano,ndpcod,ndpdsc,ndpstatus) VALUES (".$chaveseq.",".$gndcod['chave'].",'".$ano_referencia."',".$valor.",".$messagem."'N');\n";
				$dadoapoio = $chaveseq;
				$MatrizApoio[$tabela][$campo][$valor]=$dadoapoio;
				break;	
			default:
				
				//verifica campos padrão de gravação nome ou descrição
				if ( in_array(IndentificaPrefixo($nomecolunas[0])."nome",$nomecolunas2))
				{
					$campodescricao = IndentificaPrefixo($nomecolunas[0])."nome,";
					$data = getdate();
					$messagem = "'Importacao-".$data[mday]."-".$data[mon]."-".$data[year]."',";
				}
				if ( in_array(IndentificaPrefixo($nomecolunas[0])."dsc",$nomecolunas2))
				{
					$campodescricao = IndentificaPrefixo($nomecolunas[0])."dsc,";
					$data = getdate();
					$messagem = "'Importacao-".$data[mday]."-".$data[mon]."-".$data[year]."',";
				}
				
				//verifica se a chave chama cod(string) ou id(serial).	
				$x = str_replace(IndentificaPrefixo($nomecolunas[0]),"",($nomecolunas[0]));
				
				if ($x=="id")
				{
					
					$chaveseq=$db->pegaUm("select nextval('".$tabela."_".$nomecolunas[0]."_seq')");
					if ($nomecolunas[0] == $campo)
					{
						$script_apoio = "Insert into ".$tabela."(".$nomecolunas[0].",".$campodescricao.IndentificaPrefixo($nomecolunas[0])."status) VALUES (".$chaveseq.",".$messagem."'N');\n";
					}else 
					{
						$script_apoio = "Insert into ".$tabela."(".$nomecolunas[0].",".$campodescricao.IndentificaPrefixo($nomecolunas[0])."status,".$campo.") VALUES (".$chaveseq.",".$messagem."'N',".$valor.");\n";
					}
					$dadoapoio = $chaveseq;
					$MatrizApoio[$tabela][$campo][$valor]=$dadoapoio;
				}
				else 
				{
					$script_apoio = "Insert into ".$tabela."(".$nomecolunas[0].",".$campodescricao.IndentificaPrefixo($nomecolunas[0])."status) VALUES (".$valor.",".$messagem."'N');\n";
					if (substr($valor,0,1)=="'")
					{
						$dadoapoio = trim($valor,"'");
					}
					else
					{
						$dadoapoio = $valor;
					}
					$MatrizApoio[$tabela][$campo][$valor]=$dadoapoio;
					
				}
		}
		
		
		
	}
	else 
	{
		$dadoapoio = $DadosIndireta_2;
	}
	
	
	

	
	$retorno["chave"] = $dadoapoio;
	$retorno["script"]=$script_apoio;	
	

	
	/**
	 * Lista as tabelas de apoio usadas, para conferencia
	 
	global $tbapoio;
	if (!in_array($tabela,$tbapoio) and ($script_apoio != ""))
	{
		
		array_push($tbapoio,$tabela);
	}*/
	
	return $retorno;
				
}

//Criar arquivo de script e o 


function RodarQuery($caminho,$tipo,$arquivo_s='roda.sql'){
  
	$ServidorImport = "mecsrv78";
	$usuariodb = "postgres";
	rfr($caminho,$arquivo_s);
	
	if ($handle=opendir($caminho))  //abre diretório para leitura
	{
		$x=0;
		
		$matrix=glob($caminho.$tipo);
		
	}else
	{
		return false;
	}
		
	
	$arquivoscript = fopen($caminho.$arquivo_s,'w+');
	$valor = "BEGIN;\n";
	$gravar = fwrite($arquivoscript,$valor);
	
	foreach ($matrix as $arquivo)
	{
		$valor = "\\i ".$arquivo."\n";
		$gravar = fwrite($arquivoscript,$valor);
	}
	
	$valor = "COMMIT;";
	//$valor = "ROLLBACK;";
	$gravar = fwrite($arquivoscript,$valor);
	fclose($arquivoscript);
	//exec();
	
	//if (exec('psql -U '.$GLOBALS["usuario_db"].' simec -f '.$caminho.$arquivo_s) == "COMMIT")
	//if (exec('psql  -h mecsrv78 -U postgres simec -f '.$caminho.$arquivo_s) == "COMMIT")
	//dbg('/usr/bin/psql -h '.$ServidorImport.' -U '.$GLOBALS["usuario_db"].' simec -f '.$caminho.$arquivo_s,1);
	if (exec('/usr/bin/psql -h '.$ServidorImport.' -U '.$usuariodb.' simec -f '.$caminho.$arquivo_s) == "COMMIT")
	{
		return true;
	}else
	{
		return false;
	}
}

//Apaga determinado arquivo no servidor
function rfr($path,$match){
   static $deld = 0, $dsize = 0;
   $dirs = glob($path."*");
   $files = glob($path.$match);
   foreach($files as $file){
     if(is_file($file)){
         $dsize += filesize($file);
         unlink($file);
         $deld++;
     }
   }
   /*foreach($dirs as $dir){
     if(is_dir($dir)){
         $dir = basename($dir) . "/";
         rfr($path.$dir,$match);
     }
   }*/
   return "$deld files deleted with a total size of $dsize bytes";
}

//Função grava no log dados do arquivo ocorridos durante a importação
//$arquivo_import array()
//
function gravarLog($arquivo_import,$tipo=1)
{
	
	/*Grava o log do arquivos
	1 - Grava log individual, de acordo com os valores contidos na matriz $arquivo_import
	2 - Grava log Geral, Atualizando todos os log com determinada data/tipo com status de Sucesso ao rodar script
	3 - Grava log Geral, Atualizando todos os log com determinada data/tipo com status de Erro ao rodar script
	
	*/
	global $db;
	global $ErroImport;
	
	
	if(!is_null($arquivo_import['logarquivodata']))
	{
		$arquivo_import['logarquivodata'] = "cast( ".$arquivo_import['logarquivodata']." as timestamp)";
	}
	
	
	if(!is_null($arquivo_import['logdatafim']))
	{
		$arquivo_import['logdatafim'] = "cast( ".$arquivo_import['logdatafim']." as timestamp)";
	}
	
	if(!is_null($arquivo_import['logdataini']))
	{
		$arquivo_import['logdataini'] = "cast( ".$arquivo_import['logdataini']." as timestamp)";
	}
	
	if(!is_null($arquivo_import['logdataimport']))
	{
		$arquivo_import['logdataimport'] = "cast( ".$arquivo_import['logdataimport']." as timestamp)";
	}
	
	if(!is_null($arquivo_import['logdatainiscript']))
	{
		$arquivo_import['logdatainiscript'] = "cast( ".$arquivo_import['logdatainiscript']." as timestamp)";
	}
	
	if(!is_null($arquivo_import['logdatafimscript']))
	{
		$arquivo_import['logdatafimscript'] = "cast( ".$arquivo_import['logdatafimscript']." as timestamp)";
	}
	
	switch ($tipo)
	{
		case 1:
			
			$sql = "INSERT INTO importacao.logimportacao(
            logdataini, logdatafim, logarquivonome, logarquivodata, 
            logimporterros, logerros, loginformacoes, logarquivoano, logarquivotipo, 
            logatdregistros, logdataimport )
    		VALUES ( ".$arquivo_import['logdataini'].",".$arquivo_import['logdatafim'].",'".$arquivo_import['logarquivonome']."',".$arquivo_import['logarquivodata'].", 
            ".$arquivo_import['logimporterros'].",'".$arquivo_import['logerros']."',".$arquivo_import['loginformacoes'].",".$arquivo_import['logarquivoano'].",'".$arquivo_import['logarquivotipo']."', 
            ".$arquivo_import['logatdregistros'].",".$arquivo_import['logdataimport'].")";
			
			break;
		case 2:
			$sql = "UPDATE importacao.logimportacao 
   					SET logimporterros=FALSE, logerros=Null 
 					WHERE logdataimport=".$arquivo_import['logdataimport']." and logarquivotipo='".$arquivo_import['logarquivotipo']."'";
		
			//dbg($sql);
			break;
		case 3:
			$sql = "UPDATE importacao.logimportacao 
   					SET logimporterros=TRUE, logerros='".$ErroImport[12]."' 
 					WHERE logdataimport=".$arquivo_import['logdataimport']." and logarquivotipo='".$arquivo_import['logarquivotipo']."'";
			//dbg($sql);
			break;
	
		case 4:
			$sql = "UPDATE importacao.logimportacao 
   					SET logdatainiscript=".$arquivo_import['logdatainiscript'].", logdatafimscript=".$arquivo_import['logdatafimscript']."  
 					 WHERE logdataimport=".$arquivo_import['logdataimport']." and logarquivotipo='".$arquivo_import['logarquivotipo']."'";
						
			break;
		
		default:
			$sql ="";
	}
	
	
	//dbg($sql);

	$gravar = $db->carregar($sql);
		

}
// faz verificação de dados se ja contem o saldo
function verificaSaldo($chaveSaldo)
{
		
	/*IT-CO-UNIDADE-GESTORA ungcod
	IT-CO-GESTAO gstcod
	GR-CODIGO-CONTA concod
	IT-CO-CONTA-CORRENTE-CONTABIL cccnum
	IT-DA-TRANSACAO slddatatransacao
	IT-HO-TRANSACAO slddatatransacao*/
	global $BancoImportacao;
	$sql = "Select sldid from financeiro.saldocontabil where ungcod = '".$chaveSaldo['ungcod']."' and gstcod = '".$chaveSaldo['gstcod']."' and concod = '".$chaveSaldo['concod']."' and cccnum = '".$chaveSaldo['cccnum']."' and slddatatransacao = '".$chaveSaldo['slddatatransacao']."' and sldano = '".$chaveSaldo['sldano']."' and (sldmes >= '00' and  sldmes <='13')";
	//dbg($sql);
	if(($RS = pg_query($BancoImportacao,$sql)) && (pg_num_rows($RS)>=1)) {
		return $RS;
	}
	else return false;
	
	
}

//Verifica se arquivo ja foi gravado em log
function verificaLog($caminho,$nomearquivo,$ano,$tipo)
{
	
	global $db;
	$datacriacao = date("Y-m-d H:i:s",filectime($caminho.$nomearquivo));
	$sql = "Select logid,logarquivodata from importacao.logimportacao where (logarquivonome = '".$nomearquivo."') and (logarquivoano = '".$ano."') and (logarquivotipo = '".$tipo."') and (logimporterros = false)";
	
	
	$busca =$db->carregar($sql);
	if ($busca)
	{
		$teste = false;
		foreach ($busca as $registro)
		{
			
			//dbg($registro['logarquivodata'].",".$datacriacao);
			if ($registro['logarquivodata']==$datacriacao)
			{
				
				$teste =  1;
				continue;
			}else 
			{
				$teste = 2;
			}
			
			
		
		}
		
		return $teste;
		
	}else 
	{
		return false;
	}

	
}

//select cast('31/12/2006 14:59' as timestamp)


//Funções de checagem de erro

//erro de fim de arquivo
function checar_eof_file($c_arquivo)
{
		$arquivoteste = fopen($c_arquivo,'r');
		$tamanho_arquivoteste = filesize($c_arquivo);
		fseek($arquivoteste,$tamanho_arquivoteste-2);
		$fim=fgets($arquivoteste,2);
		if (ord($fim) != 10)
		{
			return true;
		}else {
			return false;
		}
		fclose($arquivoteste);
		
}

//erro de estrutura de ref
function checar_arquivo_ref($c_arquivo,$tipo)
{

	
	$array_refs = array();
	
	if (is_null($tipo)) return true;
	//array de cambio
	//$array_refs ['c'] ['IT-IN-TAXA-CONVERSAO'] = 2;
	$array_refs ['c'] ['IT-CO-MOEDA-ORIGEM'] = 3;
	$array_refs ['c'] ['IT-CO-MOEDA-DESTINO'] = 3;
	$array_refs ['c'] ['IT-DA-VIGENCIA'] = 8;
	$array_refs ['c'] ['IT-OP-CAMBIAL'] = 12;
	//$array_refs ['c'] ['IT-OP-INTEIRO'] = 3;
	//$array_refs ['c'] ['IT-OP-NUMERADOR'] = 3;
	//$array_refs ['c'] ['IT-OP-DENOMINADOR'] = 3;
	//$array_refs ['c'] ['IT-OP-CAMBIAL-AER'] = 12;
	//$array_refs ['c'] ['IT-OP-CAMBIAL-FMI'] = 12;
	//$array_refs ['c'] ['IT-OP-VALOR-MINIMO'] = 12;
	//$array_refs ['c'] ['IT-OP-VALOR-MAXIMO'] = 12;
	//$array_refs ['c'] ['IT-OP-CAMBIAL-COMPRA'] = 12;
	$array_refs ['c'] ['IT-OP-CAMBIAL-ANT'] = 12;
	
	//array de FR
	$array_refs ['fr'] ['GR-FONTE'] = 8;
	$array_refs ['fr'] ['IT-NO-FONTE'] = 45;
	//$array_refs ['fr'] ['IT-IN-TIPO-FONTE-SOF'] = 1;
	//$array_refs ['fr'] ['IT-IN-FONTE-SOF-PROGRAMACAO'] = 1;
	//$array_refs ['fr'] ['IT-IN-PERMITE-APLICACAO'] = 1;
	//$array_refs ['fr'] ['IT-IN-EXCLUSIVA-STN'] = 1;
	//$array_refs ['fr'] ['IT-IN-PERMITE-PAGAMENTO'] = 1;
	//$array_refs ['fr'] ['IT-IN-ESFERA-ORCAMENTARIA'] = 1;
	//$array_refs ['fr'] ['IT-TX-MOTIVO'] = 140;
	//$array_refs ['fr'] ['SB-IT-CO-FONTE'] = 2;
	//$array_refs ['fr'] ['SB-NO-FONTE'] = 15;
	
	//array NE
	//$array_refs ['n'] ['IT-DA-TRANSACAO'] = 8; // verificar  posteriomente
	//$array_refs ['n'] ['IT-HO-TRANSACAO'] = 4; // verificar  posteriomente
	$array_refs ['n'] ['GR-UG-GESTAO-AN-NUMERO-NEUQ(1)'] = 23;
	//$array_refs ['n'] ['GR-AN-NU-DOCUMENTO-REFERENCIA'] = 12;
	$array_refs ['n'] ['IT-DA-EMISSAO'] = 8;
	$array_refs ['n'] ['IT-CO-FAVORECIDO'] = 14;
	//$array_refs ['n'] ['IT-TX-OBSERVACAO'] = 234;
	//$array_refs ['n'] ['GR-CODIGO-EVENTO'] = 6;
	$array_refs ['n'] ['IT-IN-ESFERA-ORCAMENTARIA'] = 1;
	$array_refs ['n'] ['IT-CO-PROGRAMA-TRABALHO-RESUMIDO'] = 6;
	$array_refs ['n'] ['GR-FONTE-RECURSO'] = 10;
	$array_refs ['n'] ['GR-NATUREZA-DESPESA'] = 6;
	$array_refs ['n'] ['IT-CO-UG-RESPONSAVEL'] = 6;
	//$array_refs ['n'] ['IT-VA-TRANSACAO'] = 17;
	//$array_refs ['n'] ['IT-IN-MODALIDADE-LICITACAO'] = 2;
	//$array_refs ['n'] ['IT-NU-PROCESSO'] = 20;
	//$array_refs ['n'] ['IT-CO-UF-BENEFICIADA'] = 2;
	//$array_refs ['n'] ['IT-CO-MUNICIPIO-BENEFICIADO'] = 4;
	//$array_refs ['n'] ['IT-CO-INCISO'] = 2;
	//$array_refs ['n'] ['IT-TX-AMPARO-LEGAL'] = 8;
	//$array_refs ['n'] ['IT-CO-UG-DOC-REFERENCIA'] = 6;
	//$array_refs ['n'] ['IT-CO-GESTAO-DOC-REFERENCIA'] = 5;
	//$array_refs ['n'] ['IT-VA-CRONOGRAMA(1)'] = 17;
	//$array_refs ['n'] ['IT-IN-SITUACAO-CREDOR-SICAF'] = 2;
	//$array_refs ['n'] ['IT-DA-VENCIMENTO(1)'] = 8;
	//$array_refs ['n'] ['IT-DA-PAGAMENTO(1)'] = 8;
	//$array_refs ['n'] ['IT-VA-CRONOGRAMADO(1)'] = 17;	
	
	//$array_refs ['n'] ['IT-NU-PRECATORIO'] = 20;
	//$array_refs ['n'] ['IT-IN-PAGAMENTO-PRECATORIO'] = 1;
	//$array_refs ['n'] ['IT-NU-ORIGINAL'] = 20;
	//$array_refs ['n'] ['IT-DA-ATUALIZACAO'] = 8;
	//$array_refs ['n'] ['IT-IN-LIQUIDACAO'] = 1;
	
	//array PC
	
	//$array_refs ['pc'] ['IT-CO-USUARIO'] = 11; 
	//$array_refs ['pc'] ['IT-CO-TERMINAL-USUARIO'] = 8 ;
	//$array_refs ['pc'] ['IT-DA-TRANSACAO'] = 8;
	//$array_refs ['pc'] ['IT-HO-TRANSACAO'] = 4;
	//$array_refs ['pc'] ['IT-CO-UG-OPERADOR'] = 6;
	//$array_refs ['pc'] ['IT-IN-OPERACAO'] = 1 ;
	$array_refs ['pc'] ['GR-CODIGO-CONTA'] = 9;
	$array_refs ['pc'] ['IT-NO-CONTA'] = 45 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO-CONTA'] = 253 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(1)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(2)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(3)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(4)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(5)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(6)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(7)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(8)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(9)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(10)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(11)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(12)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(13)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(14)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(15)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(16)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(17)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(18)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(19)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-DEBITO(20)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(1)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(2)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(3)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(4)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(5)'] = 110 ;
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(6)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(7)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(8)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(9)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(10)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(11)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(12)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(13)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(14)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(15)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(16)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(17)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(18)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(19)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-CIRCUNSTANCIA-CREDITO(20)'] = 110; 
	//$array_refs ['pc'] ['IT-TX-SIGNIFICADO-SALDO'] = 210;
	//$array_refs ['pc'] ['IT-TX-OBSERVACAO-CONTA'] = 210;
	$array_refs ['pc'] ['IT-IN-CONTA-CORRENTE-CONTABIL'] = 2;
	//$array_refs ['pc'] ['IT-IN-ENCERRAMENTO'] = 1;
	//$array_refs ['pc'] ['IT-IN-INVERSAO-SALDO'] = 1;
	//$array_refs ['pc'] ['IT-IN-ESCRITURACAO'] = 1;
	$array_refs ['pc'] ['IT-IN-SALDO-CONTABIL'] = 1;
	//$array_refs ['pc'] ['IT-IN-LANCAMENTO-ORGAO'] = 1;
	//$array_refs ['pc'] ['IT-IN-INTEGRACAO'] = 1;
	//$array_refs ['pc'] ['IT-CO-EVENTO-INTEGRACAO-DEBITO'] = 6;
	//$array_refs ['pc'] ['IT-CO-EVENTO-INTEGRACAO-CREDITO'] = 6;
	//$array_refs ['pc'] ['IT-IN-SISTEMA-CONTABIL'] = 1;
	//$array_refs ['pc'] ['IT-IN-UTILIZACAO-SAFEM'] = 1;
	//$array_refs ['pc'] ['IT-IN-LANCAMENTO-NSSALDO'] = 1;
	//$array_refs ['pc'] ['IT-CO-EVENTO-NSSALDO-DEBITO'] = 6;
	//$array_refs ['pc'] ['IT-CO-EVENTO-NSSALDO-CREDITO'] = 6;
	//$array_refs ['pc'] ['IT-IN-CONTA-CAMBIO'] = 1;
	//$array_refs ['pc'] ['IT-CO-EVENTO-CAMBIO-DEBITO'] = 6;
	//$array_refs ['pc'] ['IT-CO-EVENTO-CAMBIO-CREDITO'] = 6;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(1)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(2)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(3)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(4)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(5)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(6)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(7)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(8)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(9)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(10)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(11)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(12)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(13)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(14)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(15)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(16)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(17)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(18)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(19)'] = 5;
	//$array_refs ['pc'] ['IT-CO-ORGAO-CONTA(20)'] = 5;
	//$array_refs ['pc'] ['IT-TX-MOTIVO'] = 140 ;
	//$array_refs ['pc'] ['IT-IN-RESTRICAO-CONVENIO'] = 1;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(1)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(2)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(3)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(4)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(5)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(6)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(7)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(8)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(9)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(10)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(11)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(12)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(13)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(14)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(15)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(16)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(17)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(18)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(19)'] = 70 ;
	//$array_refs ['pc'] ['IT-TX-FUNCAO(20)'] = 70 ;
	//$array_refs ['pc'] ['IT-IN-PAG-CONTRA-ENTREGA'] = 1;
	//$array_refs ['pc'] ['IT-DA-GERENCIAL'] = 8;
	//$array_refs ['pc'] ['IT-HO-GERENCIAL'] = 6;
	//$array_refs ['pc'] ['IT-IN-RESULTADO-PRIMARIO'] = 1;
	//$array_refs ['pc'] ['IT-DA-COPIA'] = 8;
	//$array_refs ['pc'] ['IT-IN-DESPESA-ESPECIAL'] = 1;
	//$array_refs ['pc'] ['IT-DA-INCLUSAO-CAMBIO'] = 8;
	$array_refs ['pc'] ['SB-NATUREZA-DESPESA'] = 8;

	//array ptres
	$array_refs ['ptres'] ['IT-CO-PROGRAMA-TRABALHO-RESUMIDO'] = 6;
	$array_refs ['ptres'] ['GR-UNIDADE-ORCAMENTARIA'] = 5;
	$array_refs ['ptres'] ['GR-PROGRAMA-TRABALHO-A'] = 17;
	//$array_refs ['ptres'] ['IT-IN-RESULTADO-PRIMARIO'] = 1;
	
	
	//array saldo
	$array_refs ['s'] ['IT-CO-UNIDADE-GESTORA'] = 6;
	$array_refs ['s'] ['IT-CO-GESTAO'] = 5;
	$array_refs ['s'] ['GR-CODIGO-CONTA'] = 9;
	$array_refs ['s'] ['IT-CO-CONTA-CORRENTE-CONTABIL'] = 43;
	$array_refs ['s'] ['IT-VA-DEBITO-INICIAL'] = 18;
	$array_refs ['s'] ['IT-VA-CREDITO-INICIAL'] = 18;
	//$array_refs ['s'] ['IT-VA-DEBITO-MENSAL(3)'] = 18;
	//$array_refs ['s'] ['IT-VA-CREDITO-MENSAL(3)'] = 18;
	$array_refs ['s'] ['IT-DA-TRANSACAO'] = 8;
	$array_refs ['s'] ['IT-HO-TRANSACAO'] = 8;

	
	//array ug
	$array_refs ['ug'] ['IT-CO-UNIDADE-GESTORA'] = 6;
	$array_refs ['ug'] ['IT-NO-UNIDADE-GESTORA'] = 45;
	$array_refs ['ug'] ['IT-NO-MNEMONICO-UNIDADE-GESTORA'] = 19;
	//$array_refs ['ug'] ['IT-IN-SITUACAO-UNIDADE-GESTORA'] = 1;
	$array_refs ['ug'] ['IT-NU-CGC'] = 14;
	$array_refs ['ug'] ['IT-CO-PAIS'] = 3;
	$array_refs ['ug'] ['IT-CO-UF'] = 2;
	$array_refs ['ug'] ['GR-ORGAO'] = 5;
	$array_refs ['ug'] ['IT-CO-UNIDADE-GESTORA-SETO-ORCA'] = 6;
	//$array_refs ['ug'] ['IT-CO-UNIDADE-GESTORA-SETO-AUDI'] = 6;
	//$array_refs ['ug'] ['IT-CO-UNIDADE-GESTORA-SETO-CONT'] = 6;
	$array_refs ['ug'] ['IT-CO-UNIDADE-GESTORA-SETO-FINA'] = 6;
	$array_refs ['ug'] ['IT-CO-UNIDADE-GESTORA-POLO'] = 6;
	//$array_refs ['ug'] ['IT-ED-ENDERECO'] = 65;
	//$array_refs ['ug'] ['IT-IN-MUDANCA-ORGAO'] = 1;
	//$array_refs ['ug'] ['IT-NU-CPF-ORDENADOR-ASS'] = 11;
	//$array_refs ['ug'] ['IT-NU-CPF-GESTOR-FINANCEIRO'] = 11;
	$array_refs ['ug'] ['IT-CO-ORGAO-SIORG'] = 6;
	$array_refs ['ug'] ['IT-CO-MOEDA'] = 3;
	$//array_refs ['ug'] ['IT-CO-MUNICIPIO'] = 4;
	$array_refs ['ug'] ['IT-CO-UG-VINCULACAO'] = 6;
	//$array_refs ['ug'] ['IT-CO-UG-CONTROLE-INTERNO'] = 6;
	$array_refs ['ug'] ['GR-UNIDADE-ORCAMENTARIA'] = 5;
	
	
	$c_arquivo = str_replace('.txt','.ref',$c_arquivo);
	$arquivo_array = file($c_arquivo);
	$campos = null;
	$campos = array();

	foreach ( $arquivo_array as $linha )
	{
		// pega definições da linha
			$linha = trim( $linha );
			
			preg_match( '/([^\s]+)[\s]{1,}[a-z]{1}[\s]{1}(.*)/i', $linha, $match );
			
			// define nome
			$nome = trim( $match[1] );
			$casas_decimais = 0;
			if ( strpos( $match[2], ',' ) !== false )
			{
				$valores = explode( ',', $match[2] );
				$casas_decimais = $valores[1];
				$match[2] = $valores[0] + $valores[1];
			}
			$tamanho = (integer) $match[2];
			
			$campos[$nome] =$tamanho;
			
	}
	
	
	foreach ($array_refs[$tipo] as $key => $value)
	{
		if ($campos [$key] != $value)
		{
			
			return false;			
		}
			
	}
	
	return true;
	
	
	
}


?>