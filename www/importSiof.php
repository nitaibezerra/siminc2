<?php

include "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

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
function tratarInput( &$traducao )
{
	rksort( $traducao );
	if ( is_array( $traducao ) == false )
	{
		return;
	}
	foreach ( $traducao as $nomeCampo => &$campo )
	{
		$quantidadeVazio = 0;
		foreach ( $campo as $key => $dados )
		{
			if ( $dados['destino'] == '' )
			{
				$quantidadeVazio++;
			}
		}
		if ( count( $campo ) == $quantidadeVazio )
		{
			unset( $traducao[$nomeCampo] );
		}
	}
}

$db = new cls_banco();

define( 'SIAF_DIR_REF_FILES', APPRAIZ . 'financeiro/arquivos/siafi/teste/' );
define( 'SIAF_DIR_TXT_FILES', APPRAIZ . 'financeiro/arquivos/siafi/teste/' );
define( 'SIAF_MAX_MONTH', 13 );

class ImportacaoSiaf
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
		$ref = $this->pegarRef( $tipo );
		
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
			
			// trata nome para os casos de campos que se repetem por meses
			if ( $nome{strlen($nome)-1} == ')' )
			{
				$nome = substr( $nome, 0, strpos( $nome, '(' ) - 1 );
			}
			
			// verifica se o campo já existe
			// ocorre em campos que se repetem por meses
			if ( array_key_exists( $nome, $campos ) == true )
			{
				// a quantidade de vezes que o campo aparece é repetida
				$campos[$nome]['repeticoes']++;
				// o início é alterado para a leitura do próximo campo
				$inicio += $campos[$nome]['tamanho'];
				continue;
			}
			
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
		// define caminho para o arquivo
		$caminho = SIAF_DIR_TXT_FILES . $arquivo;
		
		// verifica se arquivo existe
		if ( file_exists( $caminho ) == false )
		{
			return;
		}
		
		// captura sigla/abreviação
		preg_match( '/[^_]+_(.*)_[0-9]{8}\.txt/', $arquivo, $match );
		$tipo = $match[1];
		
		// carrega estrutura para realizar importação
		$estrutura = $this->pegarEstrutura( $tipo );
		
		// para a quebra linha utilizam \r\n
		$tamanhoQuebraLinha = 2;
		
		// tamanho de cada linha
		$tamanho = $estrutura['tamanho_registro'] + $tamanhoQuebraLinha;
		
		// variáveis utilizadas para fins estatísticos relativo ao arquivo
		$totalRegistros = 0;
		$totalRegistrosRepetidos = 0;
		
		// lê arquivo linha a linha
		$handle = fopen( SIAF_DIR_TXT_FILES . $arquivo, 'r' );
		while( !feof( $handle ) )
		{
			
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
						$registros[$mes][$nomeCampo] = substr( $registroBruto, $dadosCampo['inicio'],  $dadosCampo['tamanho'] );
					}
				}
			}
			
			// case não haja campos que se repetem, só existe um registro
			if ( count( $registros ) == 0 )
			{
				$registros = $camposAgrupadores;
			}
			// caso haja campos que se repetem, faz merge de cada registro com os agrupadores
			else foreach ( $registros as &$registro )
			{
				$registro = array_merge( $camposAgrupadores, $registro );
			}
			
			// realiza operação com os registros
			foreach ( $registros as $registro )
			{
				$this->importarRegistro( $registro, $tipo );
			}
			
		}
		fclose( $handle );
	}
	
	protected function importarRegistro( $registro, $tipo )
	{
		// pegar informações que trduzem para onde os dados devem ir
		var_dump( $tipo );
		var_dump( $registro );
	}
	
	/**
	 * Captura um arquivo .ref para um determinado tipo.
	 *
	 * O arquivo .ref carregado é o primeiro encontrado. Caso não exista um
	 * arquivo para o tipo determinado ou o tipo seja 'Saldo_Contabil' um texto
	 * vazio é retornado.
	 *
	 * @return string[]
	 */
	protected function pegarRef( $tipo )
	{
		// verifica se tipo é saldo contábil
		if ( $tipo == 'Saldo_Contabil' )
		{
			return array();
		}
		
		// lista arquivos do tipo
		$arquivos = glob( SIAF_DIR_REF_FILES . '*_' . $tipo . '_*.ref' );
		return count( $arquivos ) == 0 ? '' : file( current( $arquivos ) ) ;
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
	
	public function salvarParametros( $tipo, $tabela, $parametros )
	{
		$estrutura = $this->pegarEstrutura( $tipo );
		$campos = $estrutura['campos'];
		$traducao = array();
		foreach ( $parametros as $nomeCampo => $dadosCampo )
		{
			if ( array_key_exists( $nomeCampo, $campos ) == false )
			{
				continue;
			}
			$ponteiro = $campos[$nomeCampo]['inicio'];
			foreach ( $dadosCampo as $dadosItemImportacao )
			{
				if ( $dadosItemImportacao['destino'] != '' )
				{
					$novoItem = array(
						'inicio' => (integer) $ponteiro,
						'tamanho' => (integer) $dadosItemImportacao['tamanho'],
						'destino' => $dadosItemImportacao['destino'],
						'agrupador' => isset( $dadosItemImportacao['agrupador'] ) ? 1 : 0
					);
					array_push( $traducao, $novoItem );
				}
				$ponteiro += $dadosItemImportacao['tamanho'];
			}
		}
		
		// remove todas as entradas para o tipo especificado
		$sql = "DELETE FROM xxx WHERE tipo = '" . $tipo . "'";
		dbg( $sql );
		//$this->db->executar( $sql );
		$sql = "SELE FROM yyy WHERE tipo = '" . $tipo . "'";
		dbg( $sql );
		//$this->db->executar( $sql );
		
		// insere as novas entradas
		$sql = "INSERT INTO xxx ( tabela, tipo ) VALUES ( '" . $tabela . "', '" . $tipo . "' )";
		dbg( $sql );
		//$this->db->executar( $sql );
		foreach ( $traducao as $item )
		{
			$sql = "INSERT INTO xxx ( tipo, inicio, tamanho, destino, agrupador ) VALUES ( '" . $tipo . "', " . $item['inicio'] . ", " . $item['tamanho'] . ", '" . $item['destino'] . "', " . $item['agrupador'] . " )";
			dbg( $sql );
			//$this->db->executar( $sql );
		}
	}
	
}

$siaf = new ImportacaoSiaf();
//$siaf->importarArquivo( 'DOCA_NC_20060919.txt' );
//exit();

$parametros = $_REQUEST['campo'];
tratarInput( $parametros );
if ( $parametros )
{
	dbg( $parametros, 1 );
	$siaf->salvarParametros( $_REQUEST['tipoOrigem'], $_REQUEST['tabelaDestino'], $parametros );
	dbg( 1, 1 );
}

$tipoOrigem = array(
	'NC' => $siaf->pegarCampos( 'NC' ),
	'ND' => $siaf->pegarCampos( 'ND' ),
	'NE' => $siaf->pegarCampos( 'NE' ),
	'NL' => $siaf->pegarCampos( 'NL' )
);

//var_dump( $tipoOrigem, 1 );

$tabela = array();
foreach ( $db->pegarTabelas( 'financeiro' ) as $nomeTabela )
{
	$tabela[$nomeTabela] = $db->pegarColunas( $nomeTabela, 'financeiro' );
}

//	dbg( $tabela, 1 );

?>
<html>
	<head>
		
		<script type="text/javascript">
			var aux = null;
			var idDisponivel = 0;
		</script>
		
		<!-- DEFINE TIPOS PARA IMPORTACAO -->
		<script type="text/javascript">
			var tipoOrigem = new Array();
			<? foreach ( $tipoOrigem as $nomeTipo => $camposDados ) : ?>
				tipoOrigem['<?= $nomeTipo ?>'] = new Array();
				<? foreach ( $camposDados as $nomeCampo => $campoDados ) : ?>
					aux = new Array();
					aux['nome'] = '<?= $nomeCampo ?>';
					aux['tamanho'] = '<?= $campoDados['tamanho'] ?>';
					tipoOrigem['<?= $nomeTipo ?>'].push( aux );
				<? endforeach; ?>
			<? endforeach; ?>
		</script>
		<!-- FIM DEFINE TIPOS PARA IMPORTACAO -->
		
		<!-- DEFINE TABELAS -->
		<script type="text/javascript">
			var tabela = new Array();
			<? foreach ( $tabela as $nomeTabela => $camposTabela ) : ?>
				tabela['<?= $nomeTabela ?>'] = new Array();
				<? foreach ( $camposTabela as $nomeCampo ) : ?>
					tabela['<?= $nomeTabela ?>'].push( '<?= $nomeCampo ?>' );
				<? endforeach; ?>
			<? endforeach; ?>
		</script>
		<!-- FIM DEFINE TABELAS -->
		
		<script type="text/javascript">
			
			function alterarDestino()
			{
				var tabelaDestino = document.getElementById( 'tabelaDestino' ).value;
				var select = null;
				
				// atualiza combos
				var j, k, l, m, selecionado, ultimoValor, i = 0;
				while ( select = document.getElementById( 'selectDestino_' + ( i++ ) ) )
				{
					// captura o valor selecionado
					ultimoValor = '';
					m = select.options.length;
					for ( var l = 0; l < m; l++ )
					{
						if ( select.options[l].selected == true )
						{
							ultimoValor = select.options[l].value;
						}
					}
					
					// remove as opções de todos os selects
					while ( select.options.length > 0 )
					{
						select.options[0] = null;
					}
					if ( tabelaDestino == '' )
					{
						continue;
					}
					// adicona as opções
					k = tabela[tabelaDestino].length;
					select.options[0] = new Option( '', '', false, false );
//					alert( ultimoValor );
					for ( j = 0; j < k; j++ )
					{
						selecionado = ultimoValor == tabela[tabelaDestino][j];
						select.options[select.options.length] = new Option( tabela[tabelaDestino][j], tabela[tabelaDestino][j], false, selecionado );
					}
				}
				// FIM atualiza combos
			}
			
			
			function adicionarCampoOrigem( idTr, tipo, indiceOrigem )
			{
				var tabela = document.getElementById( 'tabelaTraducao' );
				var tipo = document.getElementById( 'tipoOrigem' ).value;
				var posicao = null;
				var j = tabela.rows.length;
				var posicao = null;
				for ( var i = 0; posicao < j; i++ )
				{
					if ( tabela.rows[i].id == idTr )
					{
						while( tabela.rows[++i].id.substr( 0, 13 ) == 'trImportacao_' );
						posicao = i;
						break;
					}
				}
				if ( posicao != null )
				{
					var nome = tipoOrigem[tipo][indiceOrigem]['nome'];
					var tamanho = tipoOrigem[tipo][indiceOrigem]['tamanho'];
					criarLinhaImportacao( tabela, posicao, nome, tamanho );
				}
			}
			
			function alterarTipoOrigem()
			{
				idDisponivel = 0;
				
				var tabelaTraducao = document.getElementById( 'tabelaTraducao' );
				var tipo = document.getElementById( 'tipoOrigem' ).value;
				
				// remove linhas da tabela
				
				while ( tabelaTraducao.rows.length > 2 )
				{
					tabelaTraducao.rows[2].parentNode.removeChild( tabelaTraducao.rows[2] );
				}
				
				// FIM remove linhas da tabela
				
				if ( tipo == '' )
				{
					return; // caso o tipo não exista finaliza execução
				}
				
				var i, linha, linkMais, bold, celOrigem, celAgrupador, celTamanho, celDestino, j = tipoOrigem[tipo].length;
				for ( i = 0; i < j; i++ )
				{
					linha = tabelaTraducao.insertRow( tabelaTraducao.rows.length );
					linha.id = tipoOrigem[tipo][i]['nome'];
					
					// cria celulas
					
					celOrigem = linha.insertCell( 0 );
					linkMais = document.createElement( 'a' );
					linkMais.href = 'javascript:adicionarCampoOrigem( \'' + linha.id + '\', \'' + tipo + '\',  ' + i + ' );';
					linkMais.appendChild( document.createTextNode( '[ + ]' ) );
					celOrigem.appendChild( linkMais );
					bold = document.createElement( 'b' );
					bold.appendChild( document.createTextNode( ' ' + tipoOrigem[tipo][i]['nome'] ) );
					celOrigem.appendChild( bold );
					
					celAgrupador = linha.insertCell( 1 );
					celAgrupador.style.textAlign = 'center';
					celAgrupador.appendChild( document.createTextNode( '-' ) );
					
					celTamanho = linha.insertCell( 2 );
					celTamanho.style.textAlign = 'center';
					celTamanho.appendChild( document.createTextNode( tipoOrigem[tipo][i]['tamanho'] ) );
					
					celDestino = linha.insertCell( 3 );
					celDestino.style.textAlign = 'center';
					celDestino.appendChild( document.createTextNode( ' - ' ) );
					
					// FIM cria celulas
				}
			}
			
			function criarLinhaImportacao( tabela, posicao, nome, tamanho )
			{
				linha = tabela.insertRow( posicao );
				linha.id = 'trImportacao_' + idDisponivel;
				
				// cria celula que exibe nome da campo de origem
				var celOrigem = linha.insertCell( 0 );
				celOrigem.appendChild( document.createTextNode( nome + ' ' ) );
				var linkMenos = document.createElement( 'a' );
				linkMenos.href = 'javascript:removerCampoOrigem( \'' + linha.id + '\' );';
				linkMenos.appendChild( document.createTextNode( 'remover' ) );
				celOrigem.appendChild( linkMenos );
				
				// cria celula que exibe checkbox indicando se arquivo é ou não agrupador
				var celAgrupador = linha.insertCell( 1 );
				celAgrupador.style.textAlign = 'center';
				var input = document.createElement( 'input' );
				input.type = 'checkbox';
				input.name = 'campo[' + nome + '][' + idDisponivel + '][agrupador]';
				input.value = '1';
				input.checked = false;
				celAgrupador.appendChild( input );
				
				// cria celula que define tamanho para leitura do campo
				var celTamanho = linha.insertCell( 2 );
				celTamanho.style.textAlign = 'center';
				var input = document.createElement( 'input' );
				input.type = 'text';
				input.size = '3';
				input.name = 'campo[' + nome + '][' + idDisponivel + '][tamanho]';
				input.value = tamanho;
				input.checked = false;
				celTamanho.appendChild( input );
				
				// cria celula que exibe o destino do campo origem
				var celDestino = linha.insertCell( 3 );
				celDestino.style.textAlign = 'center';
				var select = document.createElement( 'select' );
				select.id = 'selectDestino_' + idDisponivel;
				select.name = 'campo[' + nome + '][' + idDisponivel + '][destino]';
				celDestino.appendChild( select );
				
				idDisponivel++;
				alterarDestino();
			}
			
			function removerCampoOrigem( trId )
			{
				var tr = document.getElementById( trId );
				tr.parentNode.removeChild( tr );
			}
			
			
		</script>
		
		<style>
			*{ font-size: 8pt; }
			a{ text-decoration: none; }
		</style>
		
		<title>Importação SIOF</title>
	</head>
	<body>
		<form action="" method="post">
			<table width="590" id="tabelaTraducao" align="center" border="1" cellpadding="0" cellspacing="0">
				<tr>
					<td align="center">ORIGEM</td>
					<td align="center">AG.</td>
					<td align="center">TAMANHO</td>
					<td align="center">PARA</td>
				</tr>
				<tr>
					<td width="300" align="center">
						<select name="tipoOrigem" id="tipoOrigem" onchange="alterarTipoOrigem();">
							<option value=""></option>
							<? foreach ( array_keys( $tipoOrigem ) as $nomeTipo ) : ?>
								<option value="<?= $nomeTipo ?>"><?= $nomeTipo ?></option>
							<? endforeach; ?>
						</select>
					</td>
					<td width="50">&nbsp;</td>
					<td width="40">&nbsp;</td>
					<td width="200" align="center">
						<select name="tabelaDestino" id="tabelaDestino" onchange="alterarDestino();">
							<option value=""></option>
							<? foreach ( array_keys( $tabela ) as $nomeTabela ) : ?>
								<option value="<?= $nomeTabela ?>"><?= $nomeTabela ?></option>
							<? endforeach; ?>
						</select>
					</td>
				</tr>
				
			</table>
			<center>
				<br/>
				<input type="submit" name="enviar" value="enviar"/>
			</center>
		</form>
	</body>
</html>











