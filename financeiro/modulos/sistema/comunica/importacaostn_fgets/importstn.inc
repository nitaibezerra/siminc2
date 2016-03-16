<?php
/*
 * Criado em 23/04/2007
*/

set_time_limit(0);


define( 'STN_DIR_TXT_FILES', APPRAIZ . 'financeiro/arquivos/siafi/stn/' );
define ('STN_DIR_TXT_FILES_DESTINO',APPRAIZ . 'financeiro/arquivos/siafi/stn/EXECUCAO/');
$Processo_Importacao = array(); //Define o processo  de importacao foi bem definido ou não
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
 * Converte o valor para o formato compativel com o insert ou update
 */
function format_sql($valor,$tipo = null,$hora = null)
{


	$valor = trim($valor);
	$valor = str_replace("\\","/",$valor);
	//coloca null caso não for compativel ou nulo
	if ($valor=='')
	{
		return "Null";
	}else
	{
		switch ($tipo)
		{
			case 'DATA':
				$anoc = substr($valor,4,4);
				$mesc = substr($valor,2,2);
				$diac = substr($valor,0,2);
				if (is_null($hora))
				{
					return "'".$anoc."-".$mesc."-".$diac." 00:00:00"."'";
				}else
				{
					$horac = substr($hora,0,2);
					$minc = substr($hora,2,2);
					$secc = substr($hora,4,2);
					return "'".$anoc."-".$mesc."-".$diac." ".$horac.":".$minc.":".$secc."'";
				}

				break;
			case 'DECIMAL':
				return "'".(((float) $valor)/100)."'";
				break;
			default:
				return "'".$valor."'";
				break;
		}

	}
}


/*
 * Converte o valor para o formato compativel com o copy
 */
function format_copy($valor,$tipo = null,$hora = null)
{


	$valor = trim($valor);
	$valor = str_replace("\\","/",$valor);
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
				if (is_null($hora))
				{
					return $anoc."-".$mesc."-".$diac." 00:00:00";
				}else
				{
					$horac = substr($hora,0,2);
					$minc = substr($hora,2,2);
					$secc = substr($hora,4,2);
					return $anoc."-".$mesc."-".$diac." ".$horac.":".$minc.":".$secc;
				}

				break;
			case 'DECIMAL':
				return (float)($valor/100);
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
	//$ServidorImport =$GLOBALS["servidor_db"];
	//$usuariodb = "postgres";
	$usuariodb =$GLOBALS["usuario_db"];
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
	if (exec('/usr/bin/psql -h '.$ServidorImport.' -U '.$usuariodb.' simec2 -f '.$caminho.$arquivo_s) == "COMMIT")
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
function processa_arquivo($caminho,$script_nome,$Layouts)
{
	$proximo = 0;
	global $db;
	global $ErroImport;
	global $DataInicioImporta;
	global $objBar0;
	$tamanho_arquivo = filesize($caminho);
	$tamanho_atual = 0;
	ImprimeStatus("formul.txtstatus", 'Verificando o arquivo.');
	// verifica se arquivo existe
	if ( file_exists( $caminho ) == false )
	{
		ImprimeStatus("formul.txtstatus", $ErroImport[8]);
		return $ErroImport[8];
	}

	if (verificaLog($script_nome))
	{
		ImprimeStatus("formul.txtstatus", $ErroImport[5]);
		return $ErroImport[5];
	}

	//apaga arquivos de script do mesmo arquivo
	rfr(STN_DIR_TXT_FILES_DESTINO,str_replace(".txt","",$script_nome)."*.sql");


	// lê arquivo linha a linha
		ImprimeStatus("formul.txtstatus", 'Abrindo Arquivo');


		/*
		 *Solucao Ponteiro
		 */
		$handle = fopen( $caminho, 'r' );
		//fseek($handle,0);

		/*
		 * Solução Array()
		 */
		/*ImprimeStatus("formul.txtstatus", 'Abrindo Arquivo...');
		$fp = array();
		$fp = file($caminho);
		$Total_fp = sizeof($fp);*/


		$header_trabalho = "";
		$Contador_Registros_Total = 0;


		//$Navegador_fp = 0;
		while( !feof( $handle ) )
		//while ($Navegador_fp < $Total_fp)
		{
			ImprimeStatus("formul.txtstatus", 'Analisando e Criando Script');
			$linha = fgets($handle);
			//$linha = $fp[$Navegador_fp];
			$tamanho_reg = strlen($linha);
			$tamanho_reg_2 = strlen(trim($linha));
			$tamanho_atual =$tamanho_atual + $tamanho_reg;
			$proximo = muda_barra ($tamanho_arquivo,$tamanho_atual,$proximo);
			$linha=trim($linha);
			//$Navegador_fp++;

			//verifica se linha possui algum cabelho se sim procura as proximas linhas
			$header_trabalho = $Layouts[substr($linha,0,2)];
			$data_transacao_stn = format_copy(substr($linha,2,10),"DATA");
			$ano_referencia_stn = substr($linha,6,4);



			if (!is_null($header_trabalho) and (($tamanho_reg_2 == 10)or($tamanho_reg_2 == 18)))
			{


				$Contador_Registros = 0;
				//Processa Linha de acordo com Cabeçalho atual

				switch ($header_trabalho)
				{
					case 'CD':
						ImprimeStatus("formul.txtstatus","Criando script créditos descentralizados" ,Cor_Executando);
						//Carga de Creditos Descentralizados
						include 'stn_cd.inc';
						ImprimeStatus("formul.txtstatus","Finalizado criação do script" ,Cor_Executando);
						break;
					case 'SC':
						ImprimeStatus("formul.txtstatus","Criando script saldo contabil" ,Cor_Executando);
						//Carga de Creditos Descentralizados
						include 'stn_sc.inc';
						ImprimeStatus("formul.txtstatus","Finalizado criação do script" ,Cor_Executando);
						break;
					case 'NE':
						ImprimeStatus("formul.txtstatus","Criando script Nota de Empenho" ,Cor_Executando);
						//Carga de Nota de Empenho
						include 'stn_ne.inc';
						ImprimeStatus("formul.txtstatus","Finalizado criação do script" ,Cor_Executando);
						break;
					case 'TB':

						ImprimeStatus("formul.txtstatus","Criando script Tabelas de Apoio" ,Cor_Executando);
						//Carga de tabela de apoio
						include 'stn_tb.inc';
						ImprimeStatus("formul.txtstatus","Finalizado criação do script" ,Cor_Executando);
						break;
					default:
						break;
				}
				$Contador_Registros_Total = $Contador_Registros_Total + $Contador_Registros;

			}


			$objBar0->setProgress(100);

		}

		return $Contador_Registros_Total;

}

function muda_barra($total,$atual,$proximo)
{

	global $objBar0;
	if  (ceil(($atual) * 100 /ceil($total)) > ceil($proximo))
	{
		$proximo = (($atual * 100) / $total);
		$objBar0->setProgress($proximo);
	}
	return (int)$proximo;
}


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

//Verifica se arquivo ja foi gravado em log
function verificaLog($nomearquivo)
{

	global $db;
	//$datacriacao = date("Y-m-d H:i:s",filectime($caminho.$nomearquivo));
	$sql = "Select logid,logarquivodata from importacao.logimportacao where (logarquivonome = '".$nomearquivo."') and (logarquivotipo = 'stn') and (logimporterros = false)";

	$busca =$db->carregar($sql);

	if ($busca)
	{
		return true;

	}else
	{
		return false;
	}


}

function Processa_Array_Arquivos($array_arquivo)
	{
		global $objBar1;
		global $ErroImport;
		$data_importacao = date("Y-m-d");
		$dirarmazer = str_replace(" ","_",str_replace(":","",str_replace("-","", date("Y-m-d H:i:s"))));
		//Criar diretorio para armazenar os arquivos depois de executados
		if (!is_dir(STN_DIR_TXT_FILES.'/REALIZADO/'.$dirarmazer))
		{
			exec('mkdir '.STN_DIR_TXT_FILES.'/REALIZADO/'.$dirarmazer);
		}

		//Layouts que são tratados no sistema
		$Layouts['CD']='CD';
		$Layouts['SC']='SC';
		$Layouts['NE']='NE';
		$Layouts['TB']='TB';

		$quantidade_arquivos = sizeof($array_arquivo);
		$cont = 0;
		foreach ($array_arquivo as $arquivo)
		{

			$cont++;
			$objBar1->setProgress(100*$cont/$quantidade_arquivos);
			$array_arquivo_prm =array_reverse(explode("/",$arquivo));
			$arquivo_prm = $array_arquivo_prm[0];
			$caminho_prm = str_replace($array_arquivo_prm[0],"",$arquivo);
			$arquivo_nome = $arquivo_prm;
			ImprimeStatus("formul.txt".str_replace(".","_",$arquivo_nome),"Processando Arquivo..." ,Cor_Executando);
			//Dados do Log
			$data_atual = date("Y-m-d H:i:s");
			$matriz[$arquivo_nome]['logarquivonome']=$arquivo_nome; //armazena nomes dos arquivos na matriz
			$matriz[$arquivo_nome]['logarquivodata'] ="'".date("Y-m-d H:i:s", filectime($arquivo))."'";
			$matriz[$arquivo_nome]['logdatafim'] = "Null";
			$matriz[$arquivo_nome]['logdataini'] = "'".$data_atual."'";
			$matriz[$arquivo_nome]['logimporterros'] = "TRUE";
			$matriz[$arquivo_nome]['logerros'] = $ErroImport[8];
			$matriz[$arquivo_nome]['loginformacoes'] ="Null";
			$matriz[$arquivo_nome]['logarquivoano']= "Null";
			$matriz[$arquivo_nome]['logarquivotipo'] = "stn";
			$matriz[$arquivo_nome]['logatdregistros'] = 0;
			$matriz[$arquivo_nome]['caminho']="'".STN_DIR_TXT_FILES."'";
			$matriz[$arquivo_nome]['logdataimport']="'".$data_importacao."'";
			$matriz[$arquivo_nome]['logdatainiscript'] ='Null';
			$matriz[$arquivo_nome]['logdatafimscript'] ='Null';

			$valor = processa_arquivo($arquivo,$arquivo_nome,$Layouts);


			switch ($valor)
			{
				case $ErroImport[5]:
					$data_atual = date("Y-m-d H:i:s");
					$matriz[$arquivo_nome]['logdatafim'] = "'".$data_atual."'";
					$matriz[$arquivo_nome]['logerros'] = $ErroImport[5];
					gravarLog($matriz[$arquivo_nome]);
					ImprimeStatus("formul.txt".str_replace(".","_",$arquivo_nome),Cor_Erro);
					break;

				case $ErroImport[8]:
					$data_atual = date("Y-m-d H:i:s");
					$matriz[$arquivo_nome]['logdatafim'] = "'".$data_atual."'";
					$matriz[$arquivo_nome]['logerros'] = $ErroImport[8];
					gravarLog($matriz[$arquivo_nome]);
					ImprimeStatus("formul.txt".str_replace(".","_",$arquivo_nome),Cor_Erro);
					break;

				case $ErroImport[17]:
					$data_atual = date("Y-m-d H:i:s");
					$matriz[$arquivo_nome]['logdatafim'] = "'".$data_atual."'";
					$matriz[$arquivo_nome]['logerros'] = $ErroImport[17];
					gravarLog($matriz[$arquivo_nome]);
					ImprimeStatus("formul.txt".str_replace(".","_",$arquivo_nome),$ErroImport[17],Cor_Erro);
					break;

				case true:
					$matriz[$arquivo_nome]['logatdregistros'] = $valor;
					$matriz[$arquivo_nome]['logdatainiscript'] ="'".date("Y-m-d H:i:s")."'";
					$sufixo = str_replace(".txt","",$arquivo_nome);
					ImprimeStatus("formul.txtstatus","Executando o script" ,Cor_Executando);
					ImprimeStatus("formul.txt".str_replace(".","_",$arquivo_nome),"Executando o script" ,Cor_Executando);
					if (RodarQuery(STN_DIR_TXT_FILES_DESTINO,$sufixo.'*.sql', "Roda_".$sufixo.".sql"))
					{
						$matriz[$arquivo_nome]['logdatafimscript'] ="'".date("Y-m-d H:i:s")."'";
						gravarLog($matriz[$arquivo_nome],4);
						gravarLog($matriz[$arquivo_nome],2);

						exec('chmod 777 '.STN_DIR_TXT_FILES.$arquivo_nome);
						exec('mv '.STN_DIR_TXT_FILES.$arquivo_nome.' '.STN_DIR_TXT_FILES.'/REALIZADO/'.$dirarmazer);

						ImprimeStatus("formul.txt".str_replace(".","_",$arquivo_nome),"Importação realizada com sucesso" ,Cor_Sucesso);
						ImprimeStatus("formul.txtstatus","Importação realizada com sucesso" ,Cor_Sucesso);
					}else
					{
						ImprimeStatus("formul.txtstatus",$ErroImport[12],Cor_Erro);
						ImprimeStatus("formul.txt".str_replace(".","_",$arquivo_nome),$ErroImport[12],Cor_Erro);
						gravarLog($matriz[$arquivo_nome],3);
					}
					break;

				default:
					break;

			}


		}

		return true;

	}



?>

