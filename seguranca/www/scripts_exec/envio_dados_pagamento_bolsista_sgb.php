<?php
header( 'Content-Type: text/html; charset=UTF-8' );
/**
 * Script de integracao entre o PROJOVEMURBANO e SGB ( FNDE ) 
 * Tem o objetivo de buscar todos os estudantes aptos para
 * receber a bolsa e enviar os mesmos para o Sistema de Gest?o de 
 * Bolsas ( SGB ). 
 * 
 * Script agendando para ser executado semanalmente pelo 
 * agendador de scripts presente no SIMEC. 
 * 
 * Data: 07 / 08 / 2012
 * 
 * @author Arthur Claudio <arthur.almeida@squadra.com.br>
 */
define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

define( 'SISTEMA_SGB',  'PUE' );
define( 'USUARIO_SGB',  'PUE' );

// desenvolvimento
//define( 'SENHA_SGB',    'ZR1XLH2RSQHE6J5#5IKN@7FATTBJNBG$' );
// produção
define( 'SENHA_SGB',    'WRB*+WSNCWR2K!OTNX@ZKYO#E3FXG5DD' );


define( 'SGB_CNPJ_INVALIDO',    '00033' );
define( 'SGB_CNPJ_INEXISTENTE', '00039' );

// desenvolvimento
//define( 'WSDL_CAMINHO', 'http://172.20.200.162/sistema/ws?wsdl'); 
// produção
define( 'WSDL_CAMINHO', 'http://sgb.fnde.gov.br/sistema/ws/?wsdl'); 


set_time_limit( 0 );
error_reporting( E_ALL ^ E_NOTICE );

ini_set( 'soap.wsdl_cache_enabled', '0' );
ini_set( 'soap.wsdl_cache_ttl', 0 );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
$usucpf                 = '00000000191';

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/projovemurbano/_constantes.php";
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

echo date('Y-m-d H:i:s');
echo "<br/>";

function wf_alterarEstadoLocal( $docid, $aedid, $cmddsc = '', array $dados, $usucpf )
{
	global $db;
	$docid = (integer) $docid;
	$aedid = (integer) $aedid;
	$cmddsc = trim( $cmddsc );
	$cmddsc = str_replace( "'", "\\'", $cmddsc );

	$acao = wf_pegarAcao2( $aedid );
	$esdiddestino = (integer) $acao['esdiddestino'];

	// verifica se ação é possível
	if ( !wf_acaoPossivel2( $docid, $aedid, $dados ) )
	{
		return false;
	}
	
	// verifica necessidade de comentÃ¡rio
	$necessitaComentario = wf_acaoNecessitaComentario2( $aedid );
	if ( $necessitaComentario && $cmddsc == "" )
	{
		return false;
	}

	// inicia alteração de estado
	$documento = wf_pegarDocumento( $docid );
	
	// cria log no histórico
	$sqlHistorico = "
		insert into workflow.historicodocumento
		( aedid, docid, usucpf, htddata )
		values ( " . $aedid . ", " . $docid . ", '" . $usucpf . "', now() )
		returning hstid
	";
	
	$hstid = (integer) $db->pegaUm( $sqlHistorico );
   
	if ( !$hstid )
	{
		$db->rollback();
		return false;
	}
	 $db->commit();
	// cria comentário, quando necessário
	if ( $necessitaComentario )
	{
		$sqlComentario = "
			insert into workflow.comentariodocumento
			( docid, hstid, cmddsc, cmddata, cmdstatus )
			values ( " . $docid . ", " . $hstid . ", '" . addslashes($cmddsc) . "', now(), 'A' )
		";
		if ( !$db->executar( $sqlComentario ) )
		{
			$db->rollback();
			return false;
		}
		$db->commit();
	}

	// atualiza documento
	$sqlDocumento = "
		update workflow.documento
		set esdid = " . $esdiddestino . "
		where docid = " . $docid;
	
	if ( !$db->executar( $sqlDocumento ) )
	{
		$db->rollback();
		return false;
	}
	 $db->commit();
	
	// realiza pos-aÃ§Ã£o
	if ( !wf_realizarPosAcao( $aedid, $dados ) )
	{
		$db->rollback();
		return false;
	}
	
	$db->commit();
	return true;              
}

function removeAcentoGrafico ($var)
{
       $ACENTOS   = array("À","Á","Â","Ã","à","á","â","ã");
       $SEMACENTOS= array("A","A","A","A","a","a","a","a");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
      
       $ACENTOS   = array("È","É","Ê","Ë","è","é","ê","ë");
       $SEMACENTOS= array("E","E","E","E","e","e","e","e");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
       $ACENTOS   = array("Ì","Í","Î","Ï","ì","í","î","ï");
       $SEMACENTOS= array("I","I","I","I","i","i","i","i");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
      
       $ACENTOS   = array("Ò","Ó","Ô","Ö","Õ","ò","ó","ô","ö","õ");
       $SEMACENTOS= array("O","O","O","O","O","o","o","o","o","o");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
     
       $ACENTOS   = array("Ù","Ú","Û","Ü","ú","ù","ü","û");
       $SEMACENTOS= array("U","U","U","U","u","u","u","u");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
       
       $ACENTOS   = array("Ç","ç","ª","º","°", "Ñ", "´", "´´");
       $SEMACENTOS= array("C","c","a.","o.","o.", "N", "", "",);
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);      

       return $var;
}

/**
 * Retorna microtime
 * @return float
 */
function tempoExecucao(){ 
    list($usec, $sec) = explode(" ", microtime()); 
    return ((float)$usec + (float)$sec); 
}

$inicio = tempoExecucao();

/**
 * Retorna string formatada para descrever o processo realizado pelo registro
 * @param array $parametros
 * @return string
 */
function retornaFluxoDoPagamento( $parametros )
{
   $liga = false;
   //$liga = true;
    
    //Descrever processo se o ambiente for o de desenvolvimento
    if( ($_REQUEST['baselogin'] == "simec_desenvolvimento" ||  $_REQUEST['baselogin'] == "simec_espelho_producao" ) && is_array( $parametros) && $liga)
    {
        echo "\n\r";
        echo "->Tempo: "   . $parametros['tempo'];
        echo "\n\r";
        echo "Fluxo: "   . $parametros['fluxo'];
        echo "\n\r";
        echo "Descricao: " . $parametros['descricao'];
        echo "\n\r";
    }
}

//retornaFluxoDoPagamento( array( 'fluxo' => 'Inicio' , 'descricao'  => 'Inicio script', 'tempo' => tempoExecucao() - $inicio ) );

function trataTelefone( $telefone ) {

    if ( strlen( $telefone ) >= 10 ) {
        return $dadosTelefone = array(
            'nu_ddd'      => substr( $telefone, 0, 2 ),
            'nu_telefone' => substr( $telefone, 2 ),
        );
    }

    return array( 'nu_ddd'      => '01', 'nu_telefone' => $telefone );
}

function verificaRetornoSGB( $retorno ) {
    $retornoSucesso = false;
    //$retorno = utf8_decode( $retorno );

    if ( verificaCodigoRetornoSGB( $retorno, '10001' ) !== false || verificaCodigoRetornoSGB( $retorno, '10002' ) !== false ) {
        $retornoSucesso = true;
    }

    return $retornoSucesso;
}

function verificaCodigoRetornoSGB( $mensagem, $codigo ) {
    return (bool) stripos( $mensagem, $codigo ) !== false;
}

/**
 * Tramita o pagamento para o estado de 'pendente' com o retorno de erro
 * referente ao SGB.
 * 
 * @param int $pgeId
 * @param int $docId
 * @param string $motivo
 * @return bool 
 */
function retornarPagamento( $pgeId, $docId, $motivo, $usucpf ) {
    
    //echo 'Parametros: ' . $pgeId ."|". $docId ."|". $motivo ."|". $usucpf ."\n\r";
    
    $acao = wf_pegarAcao( WF_ESTADO_PAGAMENTO_AUTORIZADO, WF_ESTADO_PAGAMENTO_RECUSADO );
    return wf_alterarEstadoLocal( $docId, $acao['aedid'], $motivo, array( 'pgeid' => $pgeId ), $usucpf );
}

/**
 * Tramita o pagamento para o estado de 'enviado' ao SGB. 
 * @param int $pgeId
 * @param int $docId
 * @param string $motivo
 * @return bool 
 */
function enviarPagamento( $pgeId, $docId, $motivo, $usucpf ) {
    
    $acao = wf_pegarAcao( WF_ESTADO_PAGAMENTO_AUTORIZADO, WF_ESTADO_PAGAMENTO_ENVIADO );
    return wf_alterarEstadoLocal( $docId, $acao['aedid'], $motivo, array( 'pgeid' => $pgeId ), $usucpf );
}

retornaFluxoDoPagamento( array( 'fluxo' => 'Conectando...' , 'descricao'  => WSDL_CAMINHO, 'tempo' => tempoExecucao() - $inicio ) );

$opcoes = Array(
                'exceptions'	=> 0,
                'trace'			=> true,
                'encoding'		=> 'UTF-8',
                //'encoding'		=> 'ISO-8859-1',
                'cache_wsdl'    => WSDL_CACHE_NONE
); //
/*
                'encoding'		=> 'UTF-8',
                'proxy_host'     => "proxy.mec.gov.br",
                'proxy_port'     => 8080,
*/
        
$soapClient = new SoapClient( WSDL_CAMINHO, $opcoes );

retornaFluxoDoPagamento( array( 'fluxo' => 'Conectado' , 'descricao'  => WSDL_CAMINHO, 'tempo' => tempoExecucao() - $inicio ) );

try {
    
    libxml_use_internal_errors( true );
    
	// CPF do administrador de sistemas
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
    
    
    // abre conexção com o servidor de banco de dados
    $db = new cls_banco();

    $sqlALunos = "SELECT 
                    -- Dados do Bolsista
                    cae.caecpf, cae.caenome, cae.caesexo
                    , cae.caenomemae, cae.caenomepai, cae.caeemail
                    , cae.caetelfixo, caetelcel, endnuc.endcep as caecep
                    , endnuc.endlog as caelogradouro, coalesce((select p[1] from regexp_matches(endnuc.endnum, '[0-9]+', 'g') as p limit 1),'0')::numeric as caenumero, endnuc.endbai as caebairro
                    , endnuc.endcom as caecomp, caenumrg, caedataemissaorg, caeorgaoexpedidorrg
                    , endnuc.muncod as muncod_estudante, endnuc.estuf as estuf_estudante

                    -- Dados do Núcleo 
                    , doc.docid, tur.nucid, lpad(nab.agbcod::char(4),4,'0') as agbcod

                    -- Dados do Pagamento
                    , pge.pgeaptoreceber, pge.pgeid 
                    , lpad(date_part('month',per.perdtinicio)::char(2),2,'0')  as mes_referencia
                    , per.perperiodo as numero_parcela
                    , date_part('year',per.perdtinicio) as ano_referencia
                    , '". SISTEMA_SGB ."' as cod_programa
                    , '100.00' as valor_pagamento
                    , '52' as codigo_funcao

                    -- Dados da Entidade
                    , pju.pjuprefestuf as estuf_entidade
                    , pju.pjuprefmuncod as muncod_entidade
                    , pju.pjuprefcnpj as entnumcpfcnpj
                    , pju.pjuprefnome as entnome

                FROM projovemurbano.pagamentoestudante pge
                INNER JOIN workflow.documento doc			ON pge.docid = doc.docid 
                INNER JOIN projovemurbano.cadastroestudante cae		ON pge.caeid = cae.caeid
                INNER JOIN projovemurbano.diario dia			ON pge.diaid = dia.diaid 
                INNER JOIN workflow.documento doc2			ON dia.docid = doc2.docid
                INNER JOIN projovemurbano.periodocurso per		ON per.perid = dia.perid
                INNER JOIN projovemurbano.turma tur			ON dia.turid = tur.turid
                INNER JOIN projovemurbano.nucleo nuc			ON nuc.nucid = tur.nucid 
                INNER JOIN projovemurbano.nucleoagenciabancaria nab	ON tur.nucid = nab.nucid
                INNER JOIN projovemurbano.nucleoescola nue		ON nuc.nucid = nue.nucid AND nue.nuetipo='S' AND nue.nuestatus='A'
                INNER JOIN entidade.entidade entnuc			ON entnuc.entid = nue.entid 
                INNER JOIN entidade.endereco endnuc			ON endnuc.entid = entnuc.entid 
                INNER JOIN projovemurbano.projovemurbano pju		ON pju.pjuid = cae.pjuid
                WHERE pge.pgeaptoreceber = 't'
                AND doc.esdid = ". WF_ESTADO_PAGAMENTO_AUTORIZADO . " and per.perid=".$_REQUEST['perid']." AND doc2.esdid IN(".WF_ESTADO_DIARIO_APROVACAO.",".WF_ESTADO_DIARIO_PAGAMENTO.") order by random() ";
    //COMENTADO PARA TESTES
    //DESCOMENTAR PARA PROD
    if($_REQUEST['limit']) $sqlALunos .= ' LIMIT '.$_REQUEST['limit'];
    
    //echo( $sqlALunos );exit;

    retornaFluxoDoPagamento( array( 'fluxo' => 'Buscar todos pagamentos com situacao = AUTORIZADO', 'descricao'  => ' - ', 'tempo' => tempoExecucao() - $inicio ) );

    $alunosAptosAReceber    = $db->carregar( $sqlALunos );
    $houveAlteracaoCadastro = true;
    
    retornaFluxoDoPagamento( array( 'fluxo' => 'Query 01 - Lista de estudantes aptos a receber bolsa no periodo indicado e situacao AUTORIZADO' , 'descricao'  => 'query executada', 'tempo' => tempoExecucao() - $inicio ) );

    //Se nenhum registro for econtrado o script aborta
    if( $alunosAptosAReceber !== false ){
        
        foreach ( $alunosAptosAReceber as $aluno ) {

            $logPagamento = array( );
            $nuDDDCel  = '';
            $nuTelCel  = '';
            $nuDDDFixo = '';
            $nuTelFixo = '';

            $xmlRetorno = $soapClient->lerDadosBolsista( array(
                'sistema' => SISTEMA_SGB,
                'login'   => USUARIO_SGB,
                'senha'   => SENHA_SGB,
                'nu_cpf'  => $aluno['caecpf']
                //'nu_cpf'  => '89783930125'
                    ) );
                    
			$request  = pg_escape_string($soapClient->__getLastRequest());
			$response = pg_escape_string($soapClient->__getLastResponse());
			
			if($aluno['entnumcpfcnpj'] == '06083515000189'){
				$aluno['entnumcpfcnpj'] = '04132090000125';
			}elseif($aluno['entnumcpfcnpj'] == '01709504300109'){
					$aluno['entnumcpfcnpj']  = '17095043000109';
			}elseif($aluno['entnumcpfcnpj'] == '03088280000120'){
					$aluno['entnumcpfcnpj']  = '07974082000114';
			}elseif($aluno['entnumcpfcnpj'] == '08806721000103'){
					$aluno['entnumcpfcnpj']  = '08778326000156';
			}elseif($aluno['entnumcpfcnpj'] == '06076314000154'){
					$aluno['entnumcpfcnpj']  = '39485438000142';
			}
			
            /*    
            $sql = "INSERT INTO projovemurbano.logsgb(
					            logrequest, logresponse, logcpf, logcnpj, logdocid, logservico, 
					            logdata, logstatus, logerro, logpgeid )
					    VALUES ('".$request."', '".$response."', '".$aluno['caecpf']."', 
					    '".$aluno['entnumcpfcnpj']."', '".$aluno['docid']."', 'lerDadosBolsista', NOW(), 
					            'A',NULL, '".$aluno['pgeid']."');";
            $db->executar($sql);
            $db->commit();
            */        

            $simpleXML = new SimpleXMLElement( $xmlRetorno );

            $cpfBolsista = (string) $simpleXML->nu_cpf;
            $cpfBolsista = trim( $cpfBolsista );

            retornaFluxoDoPagamento( array( 'fluxo' => 'lerDadosBolsista' , 'descricao' => 'Metodo WS', 'tempo' => tempoExecucao() - $inicio ) );

            $dadosBolsista = array(
                'sistema'  => SISTEMA_SGB,
                'login'    => USUARIO_SGB,
                'senha'    => SENHA_SGB,
                'acao'     => 'I',
                'dt_envio' => date( 'Y-m-d' ),
                'pessoa'   => array(
                    'nu_cpf'                        => $aluno['caecpf'],
                    'no_pessoa'                     => removeAcentoGrafico( $aluno['caenome'] ),
                    'dt_nascimento'                 => $aluno['caedatanasc'],
                    'no_pai'                        => removeAcentoGrafico( $aluno['caenomepai'] ),
                    'no_mae'                        => removeAcentoGrafico( $aluno['caenomemae'] ),
                    'sg_sexo'                       => $aluno['caesexo'],
                    'co_municipio_ibge_nascimento'  => $aluno['muncod_estudante'],
                    'sg_uf_nascimento'              => $aluno['estuf_estudante'],
                    'co_estado_civil'               => 6,
                    'co_nacionalidade'              => 10,
                    'co_situacao_pessoa'            => 1,
                    'no_conjuge'                    => '',
                    'ds_endereco_web'               => '',
                    'co_agencia_sugerida'           => $aluno['agbcod'],
                    'formacoes'                     => array( ),
                    'experiencias'                  => array( ),
                    'emails'                        => array( ),
                    'telefones'                     => array( ),
                    'documentos' => array(
                        array(
                            'uf_documento'       => '',
                            'co_tipo_documento'  => 1,
                            'nu_documento'       => 'A',
                            'dt_expedicao'       => '',
                            'no_orgao_expedidor' => ''
                        )
                    ),
                    'enderecos' => array(
                        array(
                            'co_municipio_ibge'       => $aluno['muncod_estudante'],
                            'sg_uf'                   => $aluno['estuf_estudante'],
                            'ds_endereco'             => removeAcentoGrafico( $aluno['caelogradouro'] ),
                            'ds_endereco_complemento' => removeAcentoGrafico( $aluno['caecomp'] ),
                            'nu_endereco'             => removeAcentoGrafico( $aluno['caenumero'] ),
                            'nu_cep'                  => $aluno['caecep'],
                            'no_bairro'               => removeAcentoGrafico( $aluno['caebairro'] ),
                            'tp_endereco'             => 'R',
                        )
                    ),
                    'vinculacoes' => array( )
                )
            );
			/*
            if ( !empty( $aluno['caeemail'] ) ) {
                $dadosBolsista['pessoa']['emails'] = array(
                    'email' => array(
                        'ds_email' => removeAcentoGrafico( $aluno['caeemail'] )
                    )
                );
            }
			*/
			/*
            if ( !empty( $aluno['caetelfixo'] ) ) {
                $dadosTelefoneFixo = trataTelefone( $aluno['caetelcel'] );
                $nuDDDFixo         = $dadosTelefoneFixo['nu_ddd'];
                $nuTelFixo         = $dadosTelefoneFixo['nu_telefone'];

                $dadosBolsista['pessoa']['telefones'][] = array(
                    'nu_ddd_pessoa'      => $nuDDDFixo,
                    'nu_telefone_pessoa' => $nuTelFixo,
                    'tp_telefone'        => 'R'
                );
            }
			*/
			/*
            if ( !empty( $aluno['caetelcel'] ) ) {
                $dadosTelefoneCel = trataTelefone( $aluno['caetelcel'] );
                $nuDDDCel         = $dadosTelefoneCel['nu_ddd'];
                $nuTelCel         = $dadosTelefoneCel['nu_telefone'];

                $dadosBolsista['pessoa']['telefones'][] = array(
                    'nu_ddd_pessoa'      => $nuDDDCel,
                    'nu_telefone_pessoa' => $nuTelCel,
                    'tp_telefone'        => 'C'
                );
            }
			*/

            //verifica se possui o código 25 no retorno
            //indicando que o bolsista não está cadastro
            $existeCadastroSGB = ( empty( $cpfBolsista ) === false );
            //var_dump( $dadosBolsista); exit;
            
            retornaFluxoDoPagamento( array( 'fluxo' => 'Verificar se bolsista ja cadastrado no SGB', 'descricao' => 'existe cadastro? ' . $cpfBolsista, 'tempo' => tempoExecucao() - $inicio ) );
            
            //não possui cadastro no SGB
            if ( $existeCadastroSGB == false ) {
                $retornoCadastroBolsista = $soapClient->gravarDadosBolsista( $dadosBolsista );
                
                $existe_erro = verificaRetornoSGB( $retornoCadastroBolsista );
				$request  = pg_escape_string($soapClient->__getLastRequest());
				$response = pg_escape_string($soapClient->__getLastResponse());

                
                $sql = "INSERT INTO projovemurbano.logsgb(
					            logrequest, logresponse, logcpf, logcnpj, logdocid, logservico, 
					            logdata, logstatus, logerro, logpgeid )
					    VALUES ('".$request."', '".$response."', '".$aluno['caecpf']."', 
					    '".$aluno['entnumcpfcnpj']."', '".$aluno['docid']."', 'gravarDadosBolsista - Insert', NOW(), 
					            'A',".(($existe_erro)?"FALSE":"TRUE").", '".$aluno['pgeid']."');";
                
                $db->executar($sql);
                $db->commit();
                
                retornaFluxoDoPagamento( array( 'fluxo' => 'gravarDadosBolsista', 'descricao' => 'bolsista nao possui cadastro' , 'tempo' => tempoExecucao() - $inicio ) );

                if ( verificaRetornoSGB( $retornoCadastroBolsista ) == false ) {
                    retornarPagamento( $aluno['pgeid'], $aluno['docid'], (string) $retornoCadastroBolsista, $usucpf );
                    continue;
                }
            }


            if ( $existeCadastroSGB == true && $houveAlteracaoCadastro ) {

                $dadosBolsista['acao']             = 'A';
                $retornoAtualizacaoBolsista        = $soapClient->gravarDadosBolsista( $dadosBolsista );
                
                $existe_erro = verificaRetornoSGB( $retornoAtualizacaoBolsista );
               
				$request  = pg_escape_string($soapClient->__getLastRequest());
				$response = pg_escape_string($soapClient->__getLastResponse());

                
                $sql = "INSERT INTO projovemurbano.logsgb(
					            logrequest, logresponse, logcpf, logcnpj, logdocid, logservico, 
					            logdata, logstatus, logerro, logpgeid )
					    VALUES ('".$request."', '".$response."', '".$aluno['caecpf']."', 
					    '".$aluno['entnumcpfcnpj']."', '".$aluno['docid']."', 'gravarDadosBolsista - Update', NOW(), 
					            'A',".(($existe_erro)?"FALSE":"TRUE").", '".$aluno['pgeid']."');";
                $db->executar($sql);
                $db->commit();
                
                retornaFluxoDoPagamento( array( 'fluxo' => 'gravarDadosBolsista', 'descricao' => 'atualiza cadastro do Estudante' , 'tempo' => tempoExecucao() - $inicio ) );

                verificaRetornoSGB( $retornoAtualizacaoBolsista );
                $logPagamento['ATUALIZACAO_ALUNO'] = $retornoAtualizacaoBolsista;
            }

            $xmlRetornoEntidade = $soapClient->lerDadosEntidade( array('sistema'           => SISTEMA_SGB,
                                                                        'login'            => USUARIO_SGB,
                                                                        'senha'            => SENHA_SGB,
                                                                        'nu_cnpj_entidade' => $aluno['entnumcpfcnpj']
                                                                        //'nu_cnpj_entidade' => '13422776000141'
                                                                        //'nu_cnpj_entidade' => '09358108000559'
                                                                        ) );

			$request  = pg_escape_string($soapClient->__getLastRequest());
			$response = pg_escape_string($soapClient->__getLastResponse());

                
            $sql = "INSERT INTO projovemurbano.logsgb(
					            logrequest, logresponse, logcpf, logcnpj, logdocid, logservico, 
					            logdata, logstatus, logerro, logpgeid )
					    VALUES ('".$request."', '".$response."', '".$aluno['caecpf']."', 
					    '".$aluno['entnumcpfcnpj']."', '".$aluno['docid']."', 'lerDadosBolsista', NOW(), 
					            'A',NULL, '".$aluno['pgeid']."');";
            $db->executar($sql);
            $db->commit();
                                                                        

            retornaFluxoDoPagamento( array( 'fluxo' => 'Dados da Entidade', 'descricao' => 'Chama metodo lerDadosEntidade, retorna XML da Entidade' , 'tempo' => tempoExecucao() - $inicio ) );

            $objDadosEntidade = simplexml_load_string( $xmlRetornoEntidade );

            retornaFluxoDoPagamento( array( 'fluxo' => 'Entidade Cadastrada?', 'descricao' => 'Valida se entidade esta cadastrada' , 'tempo' => tempoExecucao() - $inicio ) );

            // NÃƒÂ£o possui entidade ou houve algum erro com CNPJ. 
            if ( $objDadosEntidade === false ) {

                // verifico se o cnpj informado é válido
                if ( verificaRetornoSGB( $xmlRetornoEntidade, SGB_CNPJ_INVALIDO ) == false ) {
                    retornarPagamento( $aluno['pgeid'], $aluno['docid'], (string) $xmlRetornoEntidade, $usucpf );
                    continue;
                }

                // verifica se CNPJ ÃƒÂ© Inexistente
                if ( verificaRetornoSGB( $xmlRetornoEntidade, SGB_CNPJ_INEXISTENTE ) == false ) {
                    retornarPagamento( $aluno['pgeid'], $aluno['docid'], (string) $xmlRetornoEntidade, $usucpf );
                    continue;
                }

                $dadosEntidade = array( 'sistema'          => SISTEMA_SGB,
                                        'login'            => USUARIO_SGB,
                                        'senha'            => SENHA_SGB,
                                        'nu_cnpj_entidade' => $aluno['entnumcpfcnpj'],
                                        //'nu_cnpj_entidade' => '13422776000141',
                                        'co_tipo_entidade' => '1',
                                        'no_entidade'      => $aluno['entnome'],
                                        'sg_entidade'      => '',
                                        'co_municipio'     => $aluno['muncod_entidade'],
                                        'sg_uf'            => $aluno['estuf_entidade']
                                    );

                $retornoDadosCadastroEntidade   = $soapClient->gravaDadosEntidade( $dadosEntidade );
                
				$request  = pg_escape_string($soapClient->__getLastRequest());
				$response = pg_escape_string($soapClient->__getLastResponse());
	
				$erro = verificaRetornoSGB( $retornoDadosCadastroEntidade );
	                
	            $sql = "INSERT INTO projovemurbano.logsgb(
						            logrequest, logresponse, logcpf, logcnpj, logdocid, logservico, 
						            logdata, logstatus, logerro, logpgeid )
						    VALUES ('".$request."', '".$response."', '".$aluno['caecpf']."', 
						    '".$aluno['entnumcpfcnpj']."', '".$aluno['docid']."', 'lerDadosBolsista', NOW(), 
						            'A',".(($erro)?"TRUE":"FALSE").", '".$aluno['pgeid']."');";
	            $db->executar($sql);
	            $db->commit();
                
                retornaFluxoDoPagamento( array( 'fluxo' => 'Mandar dados Entidade', 'descricao' => 'Chama metodo gravaDadosEntidade, retorno dos dados de cadastro da entidade' , 'tempo' => tempoExecucao() - $inicio ) );

                //var_dump( verificaRetornoSGB( $retornoDadosCadastroEntidade ) );exit;

                if ( verificaRetornoSGB( $retornoDadosCadastroEntidade ) == false ) {
                    retornarPagamento( $aluno['pgeid'], $aluno['docid'], (string) $retornoDadosCadastroEntidade, $usucpf );
                    continue;
                }

                /*
                * TODO
                * Armazenar em algum lugar a FLAG de "Entidade Cadastrada", sugiro a tabela projovemurbano.pagamentoestudante, ela contÃƒÂ©m o cÃƒÂ³digo do diÃƒÂ¡rio e perÃƒÂ­odo
                */
                retornaFluxoDoPagamento( array( 'fluxo' => 'Gravar Flag: Entidade cadastrada', 'descricao' => 'Altera flag de controle do cadastro da Entidade', 'tempo' => tempoExecucao() - $inicio ) );
            }


           /* $lerDadosDePagamentosPorBolsistaProgramaAnoMesReferencia = array(  'sistema'           => SISTEMA_SGB,
                                                                                'login'            => USUARIO_SGB,
                                                                                'senha'            => SENHA_SGB,
                                                                                'nu_cpf'           => $aluno['caecpf'],
                                                                                'co_programa'      => $aluno['cod_programa']);

            $retornoDadosPagamentoProgramaAnoMesReferencia = $soapClient->lerDadosDePagamentosPorBolsista( $lerDadosDePagamentosPorBolsistaProgramaAnoMesReferencia );
            var_dump( $retornoDadosPagamentoProgramaAnoMesReferencia );exit;*/

            $dadosPagamento = array(  'sistema'          => SISTEMA_SGB,
                                    'login'            => USUARIO_SGB,
                                    'senha'            => SENHA_SGB,
                                    'dt_envio'         => date('Y-m-d'),
                                    'pagamento'        => array(  'co_programa'           => $aluno['cod_programa'],
                                                                    'nu_cpf_bolsista'       => $aluno['caecpf'],
                                                                    'nu_mes_referencia'     => $aluno['mes_referencia'],
                                                                    'nu_ano_referencia'     => $aluno['ano_referencia'],
                                                                    'nu_cnpj_entidade'      => $aluno['entnumcpfcnpj'],
                                                                    'vl_pagamento'          => $aluno['valor_pagamento'],
                                                                    'nu_parcela'            => $aluno['numero_parcela'],
                                                                    'co_funcao'             => $aluno['codigo_funcao'],
                                                                    'sg_uf_atuacao'         => $aluno['estuf_estudante'],
                                                                    'co_municipio_atuacao'  => $aluno['muncod_estudante']
                                                                ) );

        /* $dadosPagamento = array(  'sistema'          => SISTEMA_SGB,
                                    'login'            => USUARIO_SGB,
                                    'senha'            => SENHA_SGB,
                                    'co_programa'      => $aluno['cod_programa'],
                                    'nu_mes_referencia'=> $aluno['mes_referencia'],
                                    'nu_ano_referencia'=> $aluno['ano_referencia'] );

            $retornoDadosPagamento = $soapClient->lerDadosDePagamentosPorPrograma( $dadosPagamento );

            var_dump( $soapClient->__getLastRequest() );
            var_dump( $soapClient->__getLastResponse() );

            */

            $retornoDadosPagamento  = $soapClient->gravaDadosPagamento( $dadosPagamento );
            
            $existe_erro = verificaRetornoSGB( $retornoDadosPagamento );
            $request  = pg_escape_string($soapClient->__getLastRequest());
			$response = pg_escape_string($soapClient->__getLastResponse());

                
                $sql = "INSERT INTO projovemurbano.logsgb(
					            logrequest, logresponse, logcpf, logcnpj, logdocid, logservico, 
					            logdata, logstatus, logerro, logpgeid )
					    VALUES ('".$request."', '".$response."', '".$aluno['caecpf']."', 
					    '".$aluno['entnumcpfcnpj']."', '".$aluno['docid']."', 'gravaDadosPagamento', NOW(), 
					            'A',".(($existe_erro)?"FALSE":"TRUE").", '".$aluno['pgeid']."');";
            $db->executar($sql);
            $db->commit();
            
            //$simpleXmlPagamento     = new SimpleXMLElement( $retornoDadosPagamento );

            retornaFluxoDoPagamento( array( 'fluxo' => 'Envia dados do bolsista para pagamento', 'descricao' => '...', 'tempo' => tempoExecucao() - $inicio ) );
            // var_dump( verificaRetornoSGB( $retornoDadosPagamento ) ); exit;
            //var_dump( $retornoDadosPagamento ); exit;

            if ( verificaRetornoSGB( $retornoDadosPagamento ) === false ) {
                retornarPagamento( $aluno['pgeid'], $aluno['docid'], (string) $retornoDadosPagamento, $usucpf );
                continue;
            }

            /*
                    // Erro no pagamento 
                    if ( $simpleXmlPagamento === false ) {

                        if ( verificaRetornoSGB( $retornoDadosPagamento ) == false ) {
                            retornarPagamento( $aluno['pgeid'], $aluno['docid'], (string) $retornoDadosPagamento, $usucpf );
                            continue;
                        }
                    }
            */

            // Atualiza o fluxo do estudante
            //( $aluno['pgeid'], $aluno['docic'], 'Enviado para pagamento' );
            enviarPagamento( $aluno['pgeid'], $aluno['docid'], 'Enviado para pagamento', $usucpf );
            /*
            * TODO
            * Definir o que serÃƒÂ¡ atualizado
            */        
            retornaFluxoDoPagamento( array( 'fluxo'         => 'Atualizar dados do pagamento do Estudante'
                                            , 'descricao'   => '..'
                                            , 'tempo'       => tempoExecucao() - $inicio ) );

            /* 
            * TODO
            * Falta entendimento sobre regra
            * --> atualizar tabela com a quantidade de  pagamentos recebidos do estudante
            * --> Vianei sabe dizer corretamente. 
            */
            retornaFluxoDoPagamento( array( 'fluxo'         => 'Soma QTD pagamento estudante na tabela cadastramentoestudante'
                                            , 'descricao'   => '..'
                                            , 'tempo'       => tempoExecucao() - $inicio ) );

            /*
                // Analisar retorno do pagamento para tratamento
                $enviadoCorretamente = true;

                if ( $enviadoCorretamente ) {
                //            $acao = wf_pegarAcao( WF_ESTADO_PAGAMENTO_AUTORIZADO, WF_ESTADO_PAGAMENTO_ENVIADO );
                //            wf_alterarEstado( $aluno['docic'], $acao['aedid'], '', array('pgeid' => $aluno['pgeid']) );
                } else {

                    $erroPagamentoAluno = '';
                }
            */

            /*
            var_dump($xmlRetornoEntidade);
            exit;
            var_dump( 2 );
            var_dump( $xmlRetornoEntidade );
            var_dump( $objDadosEntidade );
            //echo date('Y-m-d H:i:s');
            exit;
            */

            /*
                var_dump( $soapClient->__getLastRequestHeaders() );
                var_dump( $soapClient->__getLastResponseHeaders() );
                var_dump( $soapClient->__getLastRequest() );
                var_dump( $soapClient->__getLastResponse() );

                echo date('Y-m-d H:i:s');
                echo "\n\r";
                exit;
            */       
            //        
            //        
            //        
            //
            //        $simpleXMLEntidade = new SimpleXMLElement( $xmlRetornoEntidade );        
            //        
            //        
            //        $existeEntidade = (stripos( $simpleXMLEntidade->return, '0039' ) === false);
        }
    }
    
    retornaFluxoDoPagamento( array( 'fluxo'         => 'Fim do envio para pagamento'
                                    , 'descricao'   => date('Y-m-d')
                                    , 'tempo'       => tempoExecucao() - $inicio ) );

    //var_dump( 1 );
    //var_dump( $retornoDadosCadastroEntidade );

    //                var_dump( $simpleXMLCadEntidade );
    //var_dump( $soapClient->__getLastRequestHeaders() );
    //var_dump( $soapClient->__getLastResponseHeaders() );
    //exit;

    /*
    var_dump( $soapClient->__getLastRequestHeaders() );
    var_dump( $soapClient->__getLastResponseHeaders() );
    var_dump( $soapClient->__getLastRequest() );
    var_dump( $soapClient->__getLastResponse() );
    echo date('Y-m-d H:i:s');
    echo "\n\r";
    exit;
    $simpleXMLCadEntidade           = new SimpleXMLElement( $retornoDadosCadastroEntidade );
    */
    
    /*
     * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
     */
    //enviar_email( $remetente, $destinatario, $assunto, $conteudo );
    
} catch ( Exception $e ) {

    //var_dump( $soapClient->__getLastRequestHeaders() );
    //var_dump( $soapClient->__getLastResponseHeaders() );
    //var_dump( $soapClient->__getLastRequest() );
    //var_dump( $soapClient->__getLastResponse() );
    //var_dump( $e );

}

echo date('Y-m-d H:i:s');
echo "<br/>";
echo "<script>window.location=window.location;</script>";
?>