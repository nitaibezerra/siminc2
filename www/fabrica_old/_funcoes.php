<?php

require_once("_funcoes_projeto.php");
require_once("_funcoes_solicitacao.php");
require_once("_funcoes_pre_analise.php");
require_once("_funcoes_analise.php");
require_once("_funcoes_detalhamento.php");
require_once("_funcoes_avaliacao_aprovacao.php");
require_once("_funcoes_execucao.php");

/**
 * Recupera o(s) perfil(is) do usuário no módulo
 * 
 * @return array $pflcod
 */
function arrayPerfil() {
    /*     * * Variável global de conexão com o bando de dados ** */
    global $db;

    /*     * * Executa a query para recuperar os perfis no módulo ** */
    $sql    = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN 
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = " . SISID_FABRICA . "
			WHERE
				pu.usucpf = '" . $_SESSION['usucpf'] . "'
			ORDER BY
				p.pflnivel";
    $pflcod = $db->carregarColuna( $sql );

    /*     * * Retorna o array com o(s) perfil(is) ** */
    return (array) $pflcod;
}

function verificaPermissaoEdicao() {
    /*     * * Variável global de conexão com o bando de dados ** */
    global $db;

    $verifica = true;

    $pfls = arrayPerfil();

    //permissao PERFIL_REQUISITANTE
    if ( !in_array( PERFIL_SUPER_USUARIO, $pfls ) ) {

        //perfis
        if ( in_array( PERFIL_REQUISITANTE, $pfls ) 
                || in_array( PERFIL_PREPOSTO, $pfls ) 
                || in_array( PERFIL_ESPECIALISTA_SQUADRA, $pfls )  ) {
            $verifica = false;
        }

        //perfil e pagina
        $pagina = "cadDetalhamentoAnexos";
        if ( (in_array( PERFIL_PREPOSTO, $pfls ) || in_array( PERFIL_ESPECIALISTA_SQUADRA, $pfls )) && strpos( $_SESSION['favurl'], $pagina ) ) {
            $verifica = true;
        }
        $pagina   = "analiseDemandaAnexos";
        if ( (in_array( PERFIL_PREPOSTO, $pfls ) || in_array( PERFIL_ESPECIALISTA_SQUADRA, $pfls )) && strpos( $_SESSION['favurl'], $pagina ) ) {
            $verifica = true;
        }
        $pagina   = "cadOSExecucao";
        if ( (in_array( PERFIL_PREPOSTO, $pfls ) || in_array( PERFIL_ESPECIALISTA_SQUADRA, $pfls )) && strpos( $_SESSION['favurl'], $pagina ) ) {
            $verifica = true;
        }
    }

    return $verifica;
}

function enviaEmailCadSolicitacao( $scsid, $acao = null ) {
    global $db;

    if ( !$scsid ) {
        return false;
    }

    // Seta remetente
    $remetente = array( "nome"  => REMETENTE_WORKFLOW_NOME, "email" => REMETENTE_WORKFLOW_EMAIL );

    $sql = "SELECT
				s.scsid,
				u.usunome as requisitante,
				'(' || u.usufoneddd || ') ' || u.usufonenum AS telefone,
				u.usuemail as emailrequisitante,
				s.scsnecessidade,
				s.scsjustificativa,
				un.unidsc as unidade,
				s.scsprevatendimento,
				s.dataabertura,
				sis.siddescricao as sistema
				--sis.sidabrev || ' - ' || sis.siddescricao as sistema
			FROM fabrica.solicitacaoservico s
			LEFT JOIN seguranca.usuario u ON u.usucpf=s.usucpfrequisitante 
			LEFT JOIN public.unidade un ON un.unicod=u.unicod
			LEFT JOIN monitora.programa p ON p.prgid=s.prgid AND p.prgcod=s.prgcod AND p.prgano=s.prgano
			LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = s.sidid 
			WHERE s.scsid=$scsid";

    $dado = (array) $db->pegaLinha( $sql );

    //seta o destinatario
    $requisitante = $dado['emailrequisitante'];
    $destinatario = 'julioproenca@mec.gov.br';

    //pega o emails dos perfis (requisitante, preposto, fiscal e gestor do contrato)
    $sqlx = "SELECT DISTINCT
						u.usuemail
					FROM
						seguranca.usuario  AS u
					INNER JOIN 
						seguranca.perfilusuario p ON p.usucpf = u.usucpf
					INNER JOIN 
						seguranca.usuario_sistema us ON us.usucpf = u.usucpf
					WHERE 
						us.sisid = " . SISID_FABRICA . " AND
						us.suscod = 'A' AND
						p.pflcod in (" . PERFIL_PREPOSTO . ",
			 						  " . PERFIL_FISCAL_CONTRATO . ",
			 						  " . PERFIL_GESTOR_CONTRATO . ",
			 						  " . PERFIL_ESPECIALISTA_SQUADRA . ")";

    $dadox = (array) $db->carregarColuna( $sqlx );

    $dadox[]    = $requisitante;
    if ( $dadox )
        $emailCopia = $dadox; //implode("; ", $dadox);

        
//$emailCopia = 'henriquecouto@mec.gov.br';
    // seta dados da solicitacao
    $dadoSolicitacao['Nº SS']         = $dado['scsid'];
    $dadoSolicitacao['Solicitação']   = $dado['scsnecessidade'];
    $dadoSolicitacao['Justificativa'] = $dado['scsjustificativa'];

    if ( $dado['sistema'] )
        $dadoSolicitacao['Sistema'] = $dado['sistema'];


    $dadoSolicitacao['Data de abertura']           = formata_data( $dado['dataabertura'] );
    $dadoSolicitacao['Expectativa de atendimento'] = formata_data( $dado['scsprevatendimento'] );



    // seta dados do demandante
    $dadoRequisitante = array(
        "Requisitante" => $dado['requisitante'],
        "Telefone"     => $dado['telefone'],
        "Unidade"      => ($dado['unidade'] ? $dado['unidade'] : '<B>-</B>')
    );
    // Busca arquivos
    $sql           = "SELECT
				'<b>TIPO:</b> '||tasdsc||'<br><b>NOME:</b> '||arqnome||'.'||arqextensao
				||
				CASE WHEN arqdescricao <> '' THEN 
					'<br><b>DESCRIÇÃO:</b> '||arqdescricao  
				     ELSE
					''
				END AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				fabrica.anexosolicitacao a
				INNER JOIN fabrica.tipoanexosolicitacao t ON t.tasid = a.tasid
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.scsid = {$scsid}";

    $dadoArquivo = $db->carregar( $sql );
    $dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
        array(
            "arquivo" => '-',
            "data"    => '-'
        )
            );




    // Seta assunto
    if ( $acao == "ALTERAR" ) {
        $assunto       = "Nº SS [{$scsid}] – Alteração de Solicitação de Serviço POR [{$dado['requisitante']}] na DATA [" . formata_data( $dado['dataabertura'] ) . "]";
        $textoconteudo = $assunto;
    } else if ( $acao == 'PAUSA' ) {
        $assunto       = "Nº SS [{$scsid}] – Alteração de Tempo de Término de Solicitação de Serviço POR [{$dado['requisitante']}] na DATA [" . formata_data( $dado['dataabertura'] ) . "]";
        $textoconteudo = $assunto;
    } else {
        $assunto         = "Nº SS [{$scsid}] – Cadastro de Solicitação de Serviço POR [{$dado['requisitante']}] na DATA [" . formata_data( $dado['dataabertura'] ) . "]";
        $textoconteudo   = $assunto;
    }
    // Seta Conteúdo
    $conteudo        = textMail( $textoconteudo, $dadoSolicitacao, $dadoRequisitante, $dadoArquivo, $dadoHistorico   = NULL, $dadoEstadoAtual = NULL );

    enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );

    return true;
}

function enviaEmailFluxoHistorico( $scsid ) {
    global $db;
    $clsFuncoes = new funcoesProjetoFabrica();

    if ( !$scsid ) {
        return false;
    }

    // Seta remetente
    $remetente = array( "nome"  => REMETENTE_WORKFLOW_NOME, "email" => REMETENTE_WORKFLOW_EMAIL );

    $sql = "SELECT
				s.scsid,
				u.usunome as requisitante,
				'(' || u.usufoneddd || ') ' || u.usufonenum AS telefone,
				u.usuemail as emailrequisitante,
				s.scsnecessidade,
				s.scsjustificativa,
				un.unidsc as unidade,
				s.scsprevatendimento,
				s.dataabertura,
				sis.siddescricao as sistema,
				ans.ansid,
				s.docid
				--sis.sidabrev || ' - ' || sis.siddescricao as sistema
			FROM fabrica.solicitacaoservico s
			LEFT JOIN seguranca.usuario u ON u.usucpf=s.usucpfrequisitante 
			LEFT JOIN public.unidade un ON un.unicod=u.unicod
			LEFT JOIN monitora.programa p ON p.prgid=s.prgid AND p.prgcod=s.prgcod AND p.prgano=s.prgano
			LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = s.sidid 
			LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = s.scsid
			WHERE s.scsid=$scsid";

    $dado = (array) $db->pegaLinha( $sql );

    //seta o destinatario
    //$destinatario   = $dado['emailrequisitante'];
    $destinatario = 'alex.pereira@mec.gov.br';

    //pega o emails dos perfis (requisitante, preposto, fiscal e gestor do contrato)
    $sqlx = "SELECT DISTINCT
						u.usuemail
					FROM
						seguranca.usuario  AS u
					INNER JOIN 
						seguranca.perfilusuario p ON p.usucpf = u.usucpf
					INNER JOIN 
						seguranca.usuario_sistema us ON us.usucpf = u.usucpf
					WHERE 
						us.sisid = " . SISID_FABRICA . " AND
						us.suscod = 'A' AND
						p.pflcod in (" . PERFIL_PREPOSTO . ",
			 						  " . PERFIL_FISCAL_CONTRATO . ",
			 						  " . PERFIL_GESTOR_CONTRATO . ",
			 						  " . PERFIL_ESPECIALISTA_SQUADRA . ")";

    $dadox = (array) $db->carregarColuna( $sqlx );

    if ( $dadox )
        $emailCopia = $dadox; //implode("; ", $dadox);

        
//$emailCopia = 'henriquecouto@mec.gov.br';
    // seta dados da solicitacao
    $dadoSolicitacao['Nº SS']         = $dado['scsid'];
    $dadoSolicitacao['Solicitação']   = $dado['scsnecessidade'];
    $dadoSolicitacao['Justificativa'] = $dado['scsjustificativa'];

    if ( $dado['sistema'] )
        $dadoSolicitacao['Sistema'] = $dado['sistema'];


    $dadoSolicitacao['Data de abertura']           = formata_data( $dado['dataabertura'] );
    $dadoSolicitacao['Expectativa de atendimento'] = formata_data( $dado['scsprevatendimento'] );

    if ( $dado['ansid'] ) {
        $_REQUEST['ansid']              = $dado['ansid'];
        $dadoSolicitacao['Contratada']  = $clsFuncoes->listarDesciplinasOrdemServico( $tpeid                          = 1 );
        $dadoSolicitacao['Contratante'] = $clsFuncoes->listarDesciplinasOrdemServico( $tpeid                          = 2 );
    }


    // seta dados do demandante
    $dadoRequisitante = array(
        "Requisitante" => $dado['requisitante'],
        "Telefone"     => $dado['telefone'],
        "Unidade"      => ($dado['unidade'] ? $dado['unidade'] : '<B>-</B>')
    );
    // Busca arquivos
    $sql           = "SELECT
				'<b>TIPO:</b> '||tasdsc||'<br><b>NOME:</b> '||arqnome||'.'||arqextensao
				||
				CASE WHEN arqdescricao <> '' THEN 
					'<br><b>DESCRIÇÃO:</b> '||arqdescricao  
				     ELSE
					''
				END AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				fabrica.anexosolicitacao a
				INNER JOIN fabrica.tipoanexosolicitacao t ON t.tasid = a.tasid
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.scsid = {$scsid}";

    $dadoArquivo = $db->carregar( $sql );
    $dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
        array(
            "arquivo" => '-',
            "data"    => '-'
        )
            );



    // Busca historico
    $docid           = (integer) $dado['docid'];
    $dadoEstadoAtual = wf_pegarEstadoAtual( $docid );
    $dadoHistorico   = wf_pegarHistorico( $docid );


    // Seta assunto
    $nomeusucpf    = $db->pegaUm( "select usunome from seguranca.usuario where usucpf = '" . $_SESSION['usucpf'] . "'" );
    $assunto       = "Nº SS [{$scsid}] – ENVIADO PARA [{$dadoEstadoAtual['esddsc']}] POR [{$nomeusucpf}] NA DATA [" . date( 'd/m/Y' ) . "]";
    $textoconteudo = $assunto;

    // Seta Conteúdo
    $conteudo = textMail( $textoconteudo, $dadoSolicitacao, $dadoRequisitante, $dadoArquivo, $dadoHistorico, $dadoEstadoAtual['esddsc'] );

    if ( $_SERVER['HTTP_HOST'] == "localhost" || $_SERVER['HTTP_HOST'] == "simec-local" )
        return true;
    //if($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
    //	$emailCopia = 'alexpereira@mec.gov.br';
    //	$destinatario = 'henriquecouto@mec.gov.br';
    //}


    enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );

    return true;
}

function enviaEmailFluxoHistoricoOS( $odsid ) {
    global $db;

    if ( !$odsid ) {
        return false;
    }

    // Seta remetente
    $remetente = array( "nome"  => REMETENTE_WORKFLOW_NOME, "email" => REMETENTE_WORKFLOW_EMAIL );

    $sql = "SELECT
				os.odsid,
				os.scsid,
				os.docid,
				os.docidpf,
				os.odsdetalhamento,
  				os.odsqtdpfestimada,
 			    os.odsdtprevinicio,
			    os.odsdtprevtermino,
			    os.odsqtdpfdetalhada,
			    os.tosid, 
				u.usunome as requisitante,
				'(' || u.usufoneddd || ') ' || u.usufonenum AS telefone,
				u.usuemail as emailrequisitante,
				un.unidsc as unidade,
				sis.siddescricao as sistema
				--ans.ansid
				--sis.sidabrev || ' - ' || sis.siddescricao as sistema
			FROM fabrica.ordemservico os
			LEFT JOIN fabrica.solicitacaoservico s ON s.scsid = os.scsid
			LEFT JOIN seguranca.usuario u ON u.usucpf=s.usucpfrequisitante 
			LEFT JOIN public.unidade un ON un.unicod=u.unicod
			LEFT JOIN monitora.programa p ON p.prgid=s.prgid AND p.prgcod=s.prgcod AND p.prgano=s.prgano
			LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = s.sidid 
			LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = s.scsid
			WHERE os.odsid=$odsid";

    $dado = (array) $db->pegaLinha( $sql );

    //seta o destinatario
    //$destinatario   = $dado['emailrequisitante'];
    $destinatario = 'julioproenca@mec.gov.br';

    //pega o emails dos perfis (requisitante, preposto, fiscal e gestor do contrato)
    $sqlx = "SELECT DISTINCT
						u.usuemail
					FROM
						seguranca.usuario  AS u
					INNER JOIN 
						seguranca.perfilusuario p ON p.usucpf = u.usucpf
					INNER JOIN 
						seguranca.usuario_sistema us ON us.usucpf = u.usucpf
					WHERE 
						us.sisid = " . SISID_FABRICA . " AND
						us.suscod = 'A' AND
						p.pflcod in (" . ($dado['tosid'] == 1 ? PERFIL_PREPOSTO . "," . PERFIL_ESPECIALISTA_SQUADRA : PERFIL_CONTAGEM_PF) . ",
			 						  " . PERFIL_FISCAL_CONTRATO . ",
			 						  " . PERFIL_GESTOR_CONTRATO . ")";

    $dadox = (array) $db->carregarColuna( $sqlx );

    if ( $dadox )
        $emailCopia = $dadox; //implode("; ", $dadox);

        
//$emailCopia = 'henriquecouto@mec.gov.br';
    //busca profissionais envolvidos
    $profissionais = $db->carregarColuna( "SELECT distinct u.usunome || ' - ' || p.pfldsc as descricao 
											FROM seguranca.usuario u
											LEFT JOIN seguranca.perfilusuario o ON u.usucpf=o.usucpf 
											LEFT JOIN seguranca.perfil p ON p.pflcod=o.pflcod
											LEFT JOIN demandas.usuarioresponsabilidade ur ON p.pflcod=ur.pflcod AND u.usucpf=ur.usucpf
											LEFT JOIN fabrica.profissionalos pf ON u.usucpf=pf.usucpf
											WHERE sisid=" . SISID_DEMANDAS . " AND ur.celid IS NOT NULL AND ur.rpustatus='A'
											AND pf.odsid=$odsid" );




    // seta dados da solicitacao
    $dadoSolicitacao['Nº OS']                 = $dado['odsid'];
    $dadoSolicitacao['Nº SS']                 = $dado['scsid'];
    $dadoSolicitacao['Data Prevista Início']  = formata_data( $dado['odsdtprevinicio'] );
    $dadoSolicitacao['Data Prevista Término'] = formata_data( $dado['odsdtprevtermino'] );

    if ( $dado['odsqtdpfestimada'] )
        $dadoSolicitacao['Qtd. P.F. Estimada'] = number_format( $dado['odsqtdpfestimada'], 2 );

    if ( $dado['odsqtdpfdetalhada'] )
        $dadoSolicitacao['Qtd. P.F. Detalhada'] = number_format( $dado['odsqtdpfdetalhada'], 2 );

    $dadoSolicitacao['Detalhamento'] = $dado['odsdetalhamento'];

    if ( $profissionais )
        $dadoSolicitacao['Profissionais Envolvidos'] = implode( "<br>", $profissionais );
    else
        $dadoSolicitacao['Profissionais Envolvidos'] = "Nenhum";


    if ( $dado['sistema'] )
        $dadoSolicitacao['Sistema'] = $dado['sistema'];



    // seta dados do demandante
    $dadoRequisitante = array(
        "Requisitante" => $dado['requisitante'],
        "Telefone"     => $dado['telefone'],
        "Unidade"      => ($dado['unidade'] ? $dado['unidade'] : '<B>-</B>')
    );
    // Busca arquivos
    $sql           = "SELECT
				'<b>TIPO:</b> '||taodsc||'<br><b>NOME:</b> '||arqnome||'.'||arqextensao
				||
				CASE WHEN arqdescricao <> '' THEN 
					'<br><b>DESCRIÇÃO:</b> '||arqdescricao  
				     ELSE
					''
				END AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				fabrica.anexoordemservico a
				INNER JOIN fabrica.tipoanexoordem t ON t.taoid = a.taoid
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.odsid = {$odsid}";

    $dadoArquivo = $db->carregar( $sql );
    $dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
        array(
            "arquivo" => '-',
            "data"    => '-'
        )
            );



    // Busca historico
    $docid           = (integer) $dado['docid'];
    if ( $dado['docidpf'] )
        $docid           = (integer) $dado['docidpf'];
    $dadoEstadoAtual = wf_pegarEstadoAtual( $docid );
    $dadoHistorico   = wf_pegarHistorico( $docid );


    // Seta assunto
    $nomeusucpf    = $db->pegaUm( "select usunome from seguranca.usuario where usucpf = '" . $_SESSION['usucpf'] . "'" );
    $assunto       = "Nº OS [{$odsid}] ENVIADO PARA [{$dadoEstadoAtual['esddsc']}] POR [{$nomeusucpf}] NA DATA [" . date( 'd/m/Y' ) . "]";
    $textoconteudo = $assunto;

    // Seta Conteúdo
    $conteudo = textMail( $textoconteudo, $dadoSolicitacao, $dadoRequisitante, $dadoArquivo, $dadoHistorico, $dadoEstadoAtual['esddsc'] );

    if ( $_SERVER['HTTP_HOST'] == "localhost" || $_SERVER['HTTP_HOST'] == "simec-local" )
        return true;
    if ( $_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br" ) {
        $emailCopia   = 'alexpereira@mec.gov.br';
        $destinatario = 'julioproenca@mec.gov.br';
    }


    enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );

    return true;
}

/* * ************
 * Função que monta texto do email, no formato HTML.
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param  $msg (text) Texto principal do email
 * @param  $dadoDemanda (array) o índice será o label e o valor será texto que ficará à frente do label.
 * @param  $dadoDemandante (array) o índice será o label e o valor será texto que ficará à frente do label.
 * @param  $dadoArquivo (array) Será um array de array, onde os índices do array interno serão "arquivo" e "data".
 * @param  $dadoObs (array) Será um array de array, onde os índices do array interno serão "descricao" e "usuario".
 * @return (text) Texto no formato HTML;
 * @example textMail(
 * 					  'A demanda [15-2009] foi atribuida a você',
 *                    array("Data:"=>"12-10-2009"),
 *                    array("Solicitante:"=>"Felipe..."),
 *                    array(
 *                    		array("arquivo" => "arquivo.doc",
 *                    		"data" => "12/10/2009")
 *                    		),
 *                    array(
 *                    		array("descricao" => "Observações feitas...",
 *                    		"usuario" => "Felipe...")
 *                    		)
 *                   );
 *
 * ************ */

// textMail('A demanda [15-2009] foi atribuida a você', array("Data:"=>"12-10-2009"));
//function textMail($msg=null, $dadoSolicitacao = array("" => ""), $dadoRequisitante = array("-" => "-"), $dadoArquivo = array(array("arquivo" => "-", "data" => "-")), $dadoObs = array(array("descricao" => "-", "usuario" => "-", "data" => "-")), $addLinha=null ){
function textMail( $msg = null, $dadoSolicitacao = array( "" => "" ), $dadoRequisitante = array( "-" => "-" ), $dadoArquivo = array( array( "arquivo" => "-", "data"    => "-" ) ), $dadoHistorico = NULL, $dadoEstadoAtual = NULL ) {
    $text = '<html xmlns:v="urn:schemas-microsoft-com:vml" xmlns:o="urn:schemas-microsoft-com:office:office" xmlns:w="urn:schemas-microsoft-com:office:word" xmlns:m="http://schemas.microsoft.com/office/2004/12/omml" xmlns="http://www.w3.org/TR/REC-html40">
			<head>
			<style>
			.table_mail{
			    width: 80%;
			    border:outset #0099CC 2px;
			}
			
			.tit_td{
				background:#6699CC; 
				font-size:10.0pt;
				font-family:"Arial","sans-serif";
				color:white;
				font-weight: bold;
				text-align: center;
				border-bottom: 1px solid white;
				margin:2px;
			}
			
			.item_1{			
				font-size:9.0pt;
				text-align: right;
				font-weight: bold;
				font-family:"Arial","sans-serif";
				padding: 3px;
				padding-right: 5px;
				border-right: 1px solid white;
			}
			
			.item_2{
				font-size:9.0pt;			
				text-align: left;
				font-family:"Arial","sans-serif";
				padding-left: 3px;
				border-right: 1px solid white;	
			}
			
			</style>
			</head>
			<body lang=PT-BR link=blue vlink=purple>
			    <table cellpadding="1" cellspacing="0" class="table_mail">
			    	<tr>
			    		<td class="tit_td" colspan="4">' . $msg . '</td>
			    	</tr>';

    $text .= '     	<tr>
			    		<td class="tit_td" colspan="4">Dados da Solicitação de Serviço</td>
			    	</tr>';
    $a = 0;
    foreach ( $dadoSolicitacao as $ind => $val ) {
        $text .= '<tr bgcolor="' . (is_int( $a / 2 ) ? '#EFEFEF' : '#DFDFDF') . '">
		    		<td class="item_1" colspan="2" nowrap width="40%">' . $ind . ':</td>
		    		<td class="item_2" colspan="2">' . $val . '</td>
		    	  </tr>';
        $a++;
    }

    $text .= ' <tr>
		    		<td class="tit_td" colspan="4">Dados do Requisitante</td>
		    	</tr>
		    	<TR>
		    		<TD colspan="4">
		    			<table width="100%" border="0" cellpadding="0" cellspacing="0">';
    $a = 0;
    while ( list($key, $val) = each( $dadoRequisitante ) ) {
        $text .= '<tr bgcolor="' . (is_int( $a / 2 ) ? '#EFEFEF' : '#DFDFDF') . '">
		    		<td class="item_1" width="20%">' . $key . '</td>
		    		<td class="item_2" width="30%">' . $val . '</td>';

        list($key, $val) = each( $dadoRequisitante );

        $text .='
		    		<td class="item_1" width="20%">' . ($key ? $key : '-') . '</td>
		    		<td class="item_2" width="30%">' . ($val ? $val : '-') . '</td>
		    	  </tr>';
        $a++;
    }

    $text .= '
					    </table>    	   
		    		</td>
		    	</tr>';

    if ( $dadoArquivo ) {
        $text .= '
			      	<tr>
			    		<td class="tit_td" colspan="4">Dados do(s) Anexo(s)</td>
			    	</tr>
			    	<TR>
			    		<TD colspan="4">
			    			<table width="100%" border="0" cellpadding="0" cellspacing="0">    	
						    	<tr>
						    		<td class="tit_td" colspan="2">ARQUIVO</td>
						    		<td class="tit_td" colspan="2">
										DATA/HORA<BR>
										(xx-xx-xxxx xx:xx)
									</td>
						    	</tr>';
        $a = 0;
        foreach ( $dadoArquivo as $val ) {
            $text .= '<tr bgcolor="' . (is_int( $a / 2 ) ? '#EFEFEF' : '#DFDFDF') . '">
			    		<td class="item_2" colspan="2">' . $val['arquivo'] . '</td>
			    		<td class="item_2" colspan="2">' . $val['data'] . '</td>
			    	  </tr>';
            $a++;
        }

        $text .= '
					    </table>    	   
		    		</td>
		    	</tr>';
    }


    if ( $dadoHistorico ) {
        $text .= '
			    	<tr>
			    		<td class="tit_td" colspan="4">Histórico de Tramitações</td>
			    	</tr>
			    	<TR>
			    		<TD colspan="4">
			    			<table width="100%" border="0" cellpadding="0" cellspacing="0">    	
						    	<tr>
						    		<td class="tit_td">Seq.</td>
						    		<td class="tit_td">Onde Estava</td>
						    		<td class="tit_td">O que aconteceu</td>
						    		<td class="tit_td">Quem fez</td>
						    		<td class="tit_td">Quando fez</td>
						    		<td class="tit_td">Justificativa</td>
						    	</tr>';
        $a = 0;
        foreach ( $dadoHistorico as $val ) {
            $text .= '<tr bgcolor="' . (is_int( $a / 2 ) ? '#EFEFEF' : '#DFDFDF') . '">
			    		<td class="item_2">' . ($a + 1) . '</td>
			    		<td class="item_2">' . $val['esddsc'] . '</td>
			    		<td class="item_2">' . $val['aeddscrealizada'] . '</td>
			    		<td class="item_2">' . $val['usunome'] . '</td>
			    		<td class="item_2">' . $val['htddata'] . '</td>
			    		<td class="item_2">' . $val['cmddsc'] . '</td>
			    	  </tr>';
            $a++;
        }

        $text .= '<tr bgcolor="' . (is_int( $a / 2 ) ? '#EFEFEF' : '#DFDFDF') . '">
			    		<td class="item_1" colspan="6">Estado atual: <span style="color:#008000;">' . $dadoEstadoAtual . '</span></td>
			    	  </tr>';

        $text .= '
					    </table>    	   
		    		</td>
		    	</tr>';
    }




    $text .= '
					<!-- <tr>
			    		<td class="tit_td" colspan="4"> >> <a target="_blank" href="http://simec.mec.gov.br/fabrica/popSolicitacaoDetalhes.php?scsid=' . $scsid . '">CLIQUE AQUI</a> PARA ACOMPANHAR A SOLICITAÇÃO DE SERVIÇO</a> </td>
			    	</tr> -->
		    </table>
		</body>
		</html>';

    return $text;
}

function regraCancelaOS( $odsid, $tipoCusto = null ) {
    global $db;

    include_once APPRAIZ . 'includes/workflow.php';

    if ( $tipoCusto == '1' )
        $tipoCusto = true;
    else
        $tipoCusto = false;


    //pega dados da OS
    $os = $db->pegaLinha( "SELECT scsid, tosid, docid, docidpf FROM fabrica.ordemservico WHERE odsid=" . $odsid );


    $scsid = $os['scsid'];

    if ( $os['tosid'] == '2' || $os['tosid'] == '3' ) {
        $tpeid = 2;
    } else {
        $tpeid = 1;
    }

    //EMPRESA 1
    if ( $tpeid == 1 ) {

        //SemCusto = false e ComCusto = true
        if ( $tipoCusto == true ) {
            //Contagem de P.F.

            if ( $tpeid == 1 ) { // empresa 1
                $sql   = "SELECT COALESCE(os.odsqtdpfdetalhada,0) as qtdpf
							FROM fabrica.ordemservico os 
							where os.odsid = {$odsid}
							and os.tosid in (1)";
            }
            $qtdpf = $db->pegaUm( $sql );

            $valorPfFinal = $qtdpf;
            //Fim Contagem de P.F.
            //pega id do contrato
            $ctrid = $db->pegaUm( "select ctrid from fabrica.analisesolicitacao where scsid =" . $scsid );

            //Atualiza P.F. utilizado do contrato
            if ( $ctrid && $valorPfFinal ) {
                $qtd = $valorPfFinal;
                $db->executar( "UPDATE fabrica.contrato SET ctrqtdpfutilizado = ctrqtdpfutilizado + {$qtd} WHERE ctrid = {$ctrid}" );
                $db->commit();
            }


            //cancela OS com custo
            $sql   = "SELECT a.aedid
				FROM fabrica.ordemservico os
				inner join workflow.documento doc ON doc.docid = os.docid
				inner join workflow.acaoestadodoc a on a.esdidorigem = doc.esdid and a.esdiddestino=" . WF_ESTADO_OS_CANCELADA_COM_CUSTO . "
				where os.odsid = {$odsid}";
            $aedid = $db->pegaUm( $sql );

            $cmddsc = 'Ordem de Serviço Com Custo';
            $okos   = wf_alterarEstado( $os['docid'], $aedid, $cmddsc, $dados  = array( 'odsid' => $odsid ) );
        } else {

            if ( !$os['docid'] ) {
                //cria documento do WORKFLOW
                $docid = wf_cadastrarDocumento( WORKFLOW_ORDEM_SERVICO, "Fluxo de Ordem de Serviço - ID " . $odsid );

                // atualiza docid na OS
                $sql = "UPDATE
							fabrica.ordemservico
						SET 
							docid = {$docid} 
						WHERE
							odsid = {$odsid}";

                $db->executar( $sql );
                $db->commit();
            } else {
                $docid = $os['docid'];
            }

            //passa pra cancelada sem custo
            $cmddsc = 'Ordem de Serviço Sem Custo';
            $aedid  = WF_ACAO_OS_CANCELA_SEM_CUSTO;
            $okos   = wf_alterarEstado( $docid, $aedid, $cmddsc, $dados  = array( 'odsid' => $odsid ) );
        }


        if ( $okos ) {
            //envia email avisando a mudança de status da OS
            enviaEmailFluxoHistoricoOS( $odsid );
        }
    } else {

        //cancela OS tipo contagem PF (2,3) 
        $sql   = "SELECT a.aedid
			FROM fabrica.ordemservico os
			inner join workflow.documento doc ON doc.docid = os.docidpf
			inner join workflow.acaoestadodoc a on a.esdidorigem = doc.esdid and a.esdiddestino=" . WF_ESTADO_CPF_CANCELADA . "
			where os.odsid = {$odsid}";
        $aedid = $db->pegaUm( $sql );

        $cmddsc = 'Ordem de Serviço PF Cancelada';
        $okos   = wf_alterarEstado( $os['docidpf'], $aedid, $cmddsc, $dados  = array( 'odsid' => $odsid ) );

        if ( $okos ) {
            //envia email avisando a mudança de status da OS
            enviaEmailFluxoHistoricoOS( $odsid );
        }
    }



    //verifica se existe OSs em aberto para finalizar SS
    $sql = "SELECT distinct 
					odsid
					/*, 
					case when tosid = 1 then
						doc.esdid
					    else
						docpf.esdid
					end as esdid,
					tosid
					*/
			FROM fabrica.ordemservico os 
			LEFT JOIN workflow.documento doc ON doc.docid = os.docid 
			LEFT JOIN workflow.documento docpf ON docpf.docid = os.docidpf 
			WHERE scsid=" . $_SESSION['fabrica_var']['scsid'] . " 
			AND (doc.esdid not in(" . WF_ESTADO_OS_FINALIZADA . "," . WF_ESTADO_OS_CANCELADA_SEM_CUSTO . "," . WF_ESTADO_OS_CANCELADA_COM_CUSTO . ") OR doc.esdid is null) 
			AND (docpf.esdid not in(" . WF_ESTADO_CPF_FINALIZADA . "," . WF_ESTADO_CPF_CANCELADA . ") OR docpf.esdid is null) 
			limit 1";
    /*
      $sql = "SELECT esdid FROM fabrica.ordemservico os
      LEFT JOIN workflow.documento doc ON doc.docid = os.docid
      WHERE scsid=$scsid AND os.odsid not in($odsid) AND (esdid not in(".WF_ESTADO_OS_FINALIZADA.",".WF_ESTADO_OS_CANCELADA_SEM_CUSTO.",".WF_ESTADO_OS_CANCELADA_COM_CUSTO.") OR esdid is null) limit 1";
     */
    $oss = $db->pegaUm( $sql );

    if ( !$oss ) {

        //finaliza a SS somente com status em execução
        $docid = $db->pegaUm( "SELECT ss.docid FROM fabrica.solicitacaoservico ss
							  LEFT JOIN workflow.documento doc ON doc.docid = ss.docid 
							  WHERE esdid in(" . WF_ESTADO_EXECUCAO . ") and scsid=$scsid" );

        if ( $docid ) {

            $aedid  = WF_ACAO_SOL_EXECUCAOFINAL;
            $ok     = wf_alterarEstado( $docid, $aedid, $cmddsc = '', $dados  = array( 'scsid' => $scsid ) );

            if ( $ok ) {
                enviaEmailFluxoHistorico( $scsid );
            }
        }
    }


    return true;
}

function cancelaOSComCustoSemCusto( $scsid, $tipo ) {
    global $db;

    $sql = "SELECT os.odsid, os.docid, doc.esdid, a.aedid, COALESCE(os.odsqtdpfdetalhada,0) as qtdpf
			FROM fabrica.ordemservico os
			inner join workflow.documento doc ON doc.docid = os.docid
			inner join workflow.acaoestadodoc a on a.esdidorigem = doc.esdid and a.esdiddestino= $tipo
			where os.scsid = $scsid
			and os.tosid in (1)
			order by 1";
    $os  = $db->carregar( $sql );

    if ( $os ) {
        foreach ( $os as $o ) {

            $odsid  = $o['odsid'];
            $esdid  = $o['esdid'];
            $docid  = $o['docid'];
            //passa pra cancelada sem custo
            $cmddsc = 'Ordem de Serviço Sem Custo';
            $aedid  = $o['aedid'];
            $okos   = wf_alterarEstado( $docid, $aedid, $cmddsc, $dados  = array( 'odsid' => $odsid ) );

            if ( $okos ) {
                //envia email avisando a mudança de status da OS
                enviaEmailFluxoHistoricoOS( $odsid );
            }
        }
    }
}

function excluiGlosaCancelamentoComCusto( $scsid )
{
	global $db;
	
	$sql = "SELECT os.odsid, os.docid
			FROM fabrica.ordemservico os
			left join workflow.documento doc ON doc.docid = os.docid
			where os.scsid = {$scsid}
			order by 1";
	
	$os = $db->carregar( $sql );

	if ( $os ) {
		foreach ( $os as $o ) {
			
			$odsid = $o['odsid'];
			
			$sqlGlosaId = "select glosaid from fabrica.ordemservico where odsid = {$odsid}";
			$retornoSql = $db->pegaLinha( $sqlGlosaId );
			
			if( !empty( $retornoSql['glosaid'] ) ){
			
				$sql = "update fabrica.ordemservico set glosaid = null where odsid = {$odsid}";
				$db->executar( $sql );
				$db->commit();
			
				$sql = "delete from fabrica.glosa where glosaid = " . $retornoSql['glosaid'];
				$db->executar( $sql );
				$db->commit();
			}
		}
	}
}

function regraCancelaSS( $scsid = null ) {
    global $db;

    $scsid = $_SESSION['fabrica_var']['scsid'];

    if ( !$scsid )
        return false;

    include_once APPRAIZ . 'includes/workflow.php';

    $dados['scsid'] = $scsid;
    $docid          = pegarDocidSolicitacaoServico( $dados );

    //if($tipoCusto == '1') $tipoCusto = true;
    //else $tipoCusto = false;

    $tipoCusto = true;

    //Empresa 1
    $tpeid = 1;

    //EMPRESA 1
    if ( $tpeid == 1 ) {

        //(prepara para colocar pendente e depois cancelar na proxima query) - cancela todas OS sem custo da SS com docid = null
        $sql = "SELECT os.odsid, os.docid
				FROM fabrica.ordemservico os
				left join workflow.documento doc ON doc.docid = os.docid
				where os.scsid = {$scsid}
				and os.tosid in (1)
				and os.docid is null
				order by 1";

        $os = $db->carregar( $sql );
        
        if ( $os ) {
            foreach ( $os as $o ) {

                $odsid = $o['odsid'];

                // cria documento do WORKFLOW
                $docid = wf_cadastrarDocumento( WORKFLOW_ORDEM_SERVICO, "Fluxo de Ordem de Serviço - ID " . $odsid );

                // atualiza docid na OS
                $sql = "UPDATE
							fabrica.ordemservico
						SET 
							docid = {$docid} 
						WHERE
							odsid = {$odsid}";
                $db->executar( $sql );
                $db->commit();
            }
        }

        if ( $_REQUEST['esdid'] == 251 ) {
            //cancela todas OS sem custo da SS
            cancelaOSComCustoSemCusto( $scsid, WF_ESTADO_OS_CANCELADA_SEM_CUSTO );
        } else {
            //cancela todas OS com custo da SS
            cancelaOSComCustoSemCusto( $scsid, WF_ESTADO_OS_CANCELADA_COM_CUSTO );
        }

        //SemCusto = false e ComCusto = true
        if ( $tipoCusto == true ) {

            //pega id do contrato
            $ctrid = $db->pegaUm( "select ctrid from fabrica.analisesolicitacao where scsid =" . $scsid );

            //Atualiza P.F. utilizado do contrato
            if ( $ctrid && $valorPfFinal ) {
                $qtd = $valorPfFinal;
                $db->executar( "UPDATE fabrica.contrato SET ctrqtdpfutilizado = ctrqtdpfutilizado + {$qtd} WHERE ctrid = {$ctrid}" );
                $db->commit();
            }
        }

        if ( $scsid ) {
            enviaEmailFluxoHistorico( $scsid );
        }



        /*
          //cancela todas OS do tipo 2,3
          unset($os);
          $sql = "SELECT os.odsid, os.docidpf as docid, doc.esdid, a.aedid, COALESCE(os.odsqtdpfdetalhada,0) as qtdpf
          FROM fabrica.ordemservico os
          inner join workflow.documento doc ON doc.docid = os.docidpf
          inner join workflow.acaoestadodoc a on a.esdidorigem = doc.esdid and a.esdiddestino=".WF_ESTADO_CPF_CANCELADA."
          where os.scsid = {$scsid}
          and os.tosid in (2,3)
          order by 1";
          $os = $db->carregar($sql);


          if($os){

          foreach($os as $o){

          $odsid = $o['odsid'];
          $docid = $o['docid'];

          $cmddsc = 'Ordem de Serviço PF Cancelada';
          $aedid = $o['aedid'];
          $okos = wf_alterarEstado( $docid, $aedid, $cmddsc, $dados = array('odsid' => $odsid) );

          if($okos){
          //envia email avisando a mudança de status da OS
          enviaEmailFluxoHistoricoOS($odsid);

          }


          }
          }
         */





        /*
          if($tipoCusto == true){

          //cancela a SS somente com status Em Avaliação
          $docidSS = $db->pegaUm("SELECT ss.docid FROM fabrica.solicitacaoservico ss
          LEFT JOIN workflow.documento doc ON doc.docid = ss.docid
          WHERE esdid in(".WF_ESTADO_AVALIACAO.") and scsid=$scsid");

          if($docidSS){

          $aedid = WF_ACAO_SOL_CANCELA_COM_CUSTO;
          $ok = wf_alterarEstado( $docidSS, $aedid, $cmddsc = '', $dados = array('scsid' => $scsid) );

          }

          }else{

          //cancela a SS somente com status em revisao
          $docidSS = $db->pegaUm("SELECT ss.docid FROM fabrica.solicitacaoservico ss
          LEFT JOIN workflow.documento doc ON doc.docid = ss.docid
          WHERE esdid in(".WF_ESTADO_ELABORACAO.") and scsid=$scsid");

          if($docidSS){

          $aedid = WF_ACAO_SOL_CANCELA_SEM_CUSTO;
          $ok = wf_alterarEstado( $docidSS, $aedid, $cmddsc = '', $dados = array('scsid' => $scsid) );

          }

          }
         */
        excluiGlosaCancelamentoComCusto( $scsid );
    }

    return true;
}

function verificaSeTodosArtefatosPossuemRepositorioPreenchido( $idSolicitacao ) {
    global $db;
    $todosPossuemRepositorio = true;

    //SQL de ServicoFaseProdutoRepositorio::recupereContratadosOrdenadosPorFaseDisciplinaPeloIdSolicitacao($idSolicitacao);
    $sql = "SELECT fdp.prdid,
					pro.prddsc,
					pro.prdstatus,
					fdp.fdpid,
					fd.fsddsc,
					fd.fsdid,
					sfp.sfpid,
					dis.dspid,
					sfp.sfprepositorio,
					sfp.sfpadraonome,
					sfp.sfppadraodiretorio,
					sfp.sfpencontrado,
					sfp.sfpatualizado,
					sfp.sfpnecessario
				FROM fabrica.fasedisciplinaproduto fdp
				JOIN fabrica.fasedisciplina fd
					ON fdp.fsdid = fd.fsdid
				JOIN fabrica.produto pro
					ON fdp.prdid = pro.prdid
				JOIN fabrica.disciplina dis
					ON dis.dspid = fd.dspid
				JOIN fabrica.servicofaseproduto sfp
					ON sfp.fdpid = fdp.fdpid
				JOIN fabrica.analisesolicitacao ans
					ON ans.ansid = sfp.ansid
				WHERE pro.prdid <> 44 and ans.scsid = $idSolicitacao and sfp.tpeid = 1
				ORDER BY fd.fsdid";

    $resultSet = $db->carregar( $sql );
    foreach ( $resultSet as $linha ) {
        $todosPossuemRepositorio = $todosPossuemRepositorio && $linha['sfprepositorio'] != '';
    }
    return $todosPossuemRepositorio;
}

function verificaSeTodosArtefatosForamAceitos( $idSolicitacao ) {

    global $db;
    $sql = "SELECT fdp.prdid,
					pro.prddsc,
					pro.prdstatus,
					fdp.fdpid,
					fd.fsddsc,
					fd.fsdid,
					sfp.sfpid,
					dis.dspid,
					sfp.sfprepositorio,
					sfp.sfpadraonome,
					sfp.sfppadraodiretorio,
					sfp.sfpencontrado,
					sfp.sfpatualizado,
					sfp.sfpnecessario,
					dtaud.dtaresultado
				FROM fabrica.fasedisciplinaproduto fdp
				JOIN fabrica.fasedisciplina fd
					ON fdp.fsdid = fd.fsdid
				JOIN fabrica.produto pro
					ON fdp.prdid = pro.prdid
				JOIN fabrica.disciplina dis
					ON dis.dspid = fd.dspid
				JOIN fabrica.servicofaseproduto sfp
					ON sfp.fdpid = fdp.fdpid
				JOIN fabrica.analisesolicitacao ans
					ON ans.ansid = sfp.ansid
				LEFT JOIN fabrica.detalhesauditoria dtaud
					ON dtaud.sfpid = sfp.sfpid
				WHERE pro.prdid <> 44 and ans.scsid = $idSolicitacao and sfp.tpeid = 1
				ORDER BY fd.fsdid";

    $artefatosAceitos = true;

    $resultSet = $db->carregar( $sql );
    if ( $resultSet != null ) {
        foreach ( $resultSet as $linha ) {
            $artefatosAceitos = $artefatosAceitos && ($linha['dtaresultado'] == 1 || $linha['dtaresultado'] == 2 || $linha['dtaresultado'] == 3);
        }
    } else {
        return false;
    }

    return $artefatosAceitos;
}

function verificaSeHaAlgumArtefatoSemAuditoria( $idSolicitacao ) {
    return true;
}

function recuperaMetrica( $ansid = null ){
	global $db;
	
	$sql = "select distinct
                mt.mtcsigla
          from 
                fabrica.analisesolicitacao an
          inner join fabrica.metricaitem mi on mi.mtiid = an.mtiid
          inner join fabrica.metrica mt on mt.mtcid = mi.mtcid
          WHERE ansid=".$ansid;
	$sigla = $db->pegaUm($sql);
	
	return $sigla;
}
?>