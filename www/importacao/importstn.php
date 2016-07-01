<?php
/*
 * Criado em 23/04/2007
*/

set_time_limit(0);

include "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

define ('STN_DIR_TXT_FILES',APPRAIZ . 'financeiro/arquivos/siafi/stn/');
define ('STN_DIR_TXT_FILES_DESTINO',APPRAIZ . 'financeiro/arquivos/siafi/stn/EXECUCAO/');

$db = new cls_banco();
$Processo_Importacao = array(); //Define o processo  de importacao foi bem definido ou não
$Layouts["CD"] = "CD";//Layouts que estão sendo processados
$matriz = array();//matriz que guarda informações do arquivo para log
$DataInicioImporta = date("Y-m-d H:i:s"); //Data do incio da importacao
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
  $ErroImport[17] = "Erro n Criação do Arquivo";

  $sucessoImport = "Importação Concluida com Sucesso";


/*
 * Converte o valor para o formato compativel com o copy
 */
function format_copy($valor,$tipo = null)
{


	$valor = trim($valor);
	//coloca null caso não for compativel ou nulo
	if ($valor=='')
	{
		return "\\N";
	}else
	{
		switch ($tipo)
		{
			case 'DATA':
				$anoc = substr($valor,4,4);
				$mesc = substr($valor,2,2);
				$diac = substr($valor,0,2);
				return $anoc."-".$mesc."-".$diac;
				break;
			case 'DECIMAL':
				return ((float) $valor)/100;
				break;
			default:
				return $valor;
				break;
		}

	}
}

/*
 * Criar arquivo de script e o executa
 */
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

	//$valor = "COMMIT;";
	$valor = "ROLLBACK;";
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

/*
 *Gravar Log de Execução
 */
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


/*
 * Verifica se ja foi gravado no Log
 */
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




/*
 * Apaga determinado arquivo no servidor
 */
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



/*
 * Processa Arquivo vindo do STN, com deternados tipos
 */
function processa_arquivo($arquivo_nome,$Layouts)
{

	global $db;
	global $ErroImport;
	global $DataInicioImporta;
	$caminho = STN_DIR_TXT_FILES.$arquivo_nome;

	// verifica se arquivo existe
	if ( file_exists( $caminho ) == false )
	{
		return $ErroImport[8];
	}

	//apaga arquivos de script do mesmo arquivo
	rfr(STN_DIR_TXT_FILES_DESTINO,str_replace(".txt","",$arquivo_nome)."*.sql");


	// lê arquivo linha a linha
		$totalRegistros = 0;
		$handle = fopen( $caminho, 'r' );
		$header_trabalho = "";
		$Contador_Registros_Total =0;
		while( !feof( $handle ) )
		{
			$linha = trim(fgets($handle));
			$tamanho_reg = strlen($linha);


			//verifica se linha possui algum cabelho se sim procura as proximas linhas
			$header_trabalho = $Layouts[substr($linha,0,2)];
			$data_transacao_stn = format_copy(substr($linha,2,10),"DATA");
			$ano_referencia_stn = substr($linha,6,4);

			if (!is_null($header_trabalho) and (($tamanho_reg == 10)or($tamanho_reg == 18)))
			{
				$Contador_Registros = 0;
				//Processa Linha de acordo com Cabeçalho atual
				switch ($header_trabalho)
				{
					case 'CD':
						//Carga de Creditos Descentralizados
						include 'stn_dc.inc';
						break;
					default:
						break;
				}
				$Contador_Registros_Total = $Contador_Registros_Total + $Contador_Registros;

			}
			$totalRegistros++;
		}

		return $Contador_Registros_Total;

}





	//Dados do Log
	$arquivo_nome ='credito_desc_mec27032007.txt';
	$matriz[$arquivo_nome]['logarquivonome']=$arquivo_nome; //armazena nomes dos arquivos na matriz
	$matriz[$arquivo_nome]['logarquivodata'] ="'".date("Y-m-d H:i:s", filectime(STN_DIR_TXT_FILES.$arquivo_nome))."'";
	$data_atual = date("Y-m-d H:i:s");
	$matriz[$arquivo_nome]['logdatafim'] = "Null";
	$matriz[$arquivo_nome]['logdataini'] = "'".$data_atual."'";
	$matriz[$arquivo_nome]['logimporterros'] = "TRUE";
	$matriz[$arquivo_nome]['logerros'] = $ErroImport[8];
	$matriz[$arquivo_nome]['loginformacoes'] ="Null";
	$matriz[$arquivo_nome]['logarquivoano']= "Null";
	$matriz[$arquivo_nome]['logarquivotipo'] = "stn";
	$matriz[$arquivo_nome]['logatdregistros'] = 0;
	$matriz[$arquivo_nome]['caminho']="'".STN_DIR_TXT_FILES."'";
	$matriz[$arquivo_nome]['logdataimport']="'".$DataInicioImporta."'";
	$matriz[$arquivo_nome]['logdatainiscript'] ='Null';
	$matriz[$arquivo_nome]['logdatafimscript'] ='Null';





//Executa codigo
$valor = processa_arquivo($arquivo_nome,$Layouts);
switch ($valor)
{
	case $ErroImport[8]:
		$data_atual = date("Y-m-d H:i:s");
		$matriz[$arquivo_nome]['logdatafim'] = "'".$data_atual."'";
		$matriz[$arquivo_nome]['logerros'] = $ErroImport[8];
		gravarLog($matriz[$arquivo_nome]);
		break;

	case $ErroImport[17]:
		$data_atual = date("Y-m-d H:i:s");
		$matriz[$arquivo_nome]['logdatafim'] = "'".$data_atual."'";
		$matriz[$arquivo_nome]['logerros'] = $ErroImport[17];
		gravarLog($matriz[$arquivo_nome]);
		break;

	case true:
		$matriz[$arquivo_nome]['logatdregistros'] = $valor;
		$matriz[$arquivo_nome]['logdatainiscript'] ="'".date("Y-m-d H:i:s")."'";
		$sufixo = str_replace("*.txt","",$arquivo_nome);
		if (RodarQuery(STN_DIR_TXT_FILES_DESTINO,$sufixo.'*.sql', "Roda_".$sufixo.".sql"))
		{
			$matriz[$arquivo_nome]['logdatafimscript'] ="'".date("Y-m-d H:i:s")."'";
			gravarLog($matriz[$arquivo_nome],4);
			gravarLog($matriz[$arquivo_nome],2);
		}else
		{
			gravarLog($matriz[$arquivo_nome],3);
		}
		break;

	default:
		break;

}

?>
