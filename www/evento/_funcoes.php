<?PHP

include_once APPRAIZ . "includes/classes/dateTime.inc";
require_once APPRAIZ . "includes/Email.php";
function pegaPerfil( $usucpf )
{
	global $db;
	$sql = "SELECT pu.pflcod
			FROM seguranca.perfil AS p
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE p.sisid = '{$_SESSION['sisid']}'
			AND
			pu.usucpf = '$usucpf'";


	$pflcod = $db->pegaUm( $sql );
	return $pflcod;
}
function redirecionar( $modulo, $acao, $parametros = array() )
{
    $parametros = http_build_query( (array) $parametros, '', '&' );
    header( "Location: ?modulo=$modulo&acao=$acao&$parametros" );
    exit();
}

function headEvento($title, $gestor, $unidadeDemandante, $condicao, $adreferendum, $dtInclusao, $valores = true) {
    global $db;

    $dtInclusao = formata_data($dtInclusao);

    if ($_SESSION['evento']['eveid']){

        $data_nova_regra_evento = strtotime(DATA_NOVO_REGRA_EVENTO_NOVA_INFRA);
        $data_inclusao_evento = strtotime($_SESSION['evento']['evedatainclusao']);

        if( $data_inclusao_evento < $data_nova_regra_evento ){
            $sql = "
                SELECT  evecustoprevisto AS preveisto,
                        urevalorsaldo AS saldo
                FROM evento.unidaderecurso_old AS ur
                INNER JOIN evento.evento AS ev ON ev.ureid = ur.ureid
                WHERE ev.eveid = {$_SESSION['evento']['eveid']}
            ";
            $arrFinanceiroEvento = $db->pegaLinha($sql);
            $titulo_descricao = "Saldo da Unidade:";
        }else{
            $sql = "
                SELECT  emp.empsaldoinicontrato AS saldo_inicial,
                        emp.empvalorutilizado AS valor_utilisado,
                        eve.evecustoprevisto AS custo_prev
                        --,total.total_prev AS total_prev

                FROM evento.empenho_unidade AS emp
                JOIN evento.evento AS eve ON eve.emuid = emp.emuid

                JOIN(
                        SELECT emuid, sum(evecustoprevisto) AS total_prev FROM evento.evento GROUP BY emuid
                ) AS total ON total.emuid = emp.emuid

                WHERE eve.eveid =  {$_SESSION['evento']['eveid']}
            ";
            $arrFinanceiroEvento = $db->pegaLinha($sql);
            $titulo_descricao = "Saldo Empenho da Unidade:";
        }

        $saldos = '';
    }

    $cab = "
        <table align=\"center\" class=\"tabela\">
            <tr>
                <td width=\"65%\">
                    <table align=\"center\" class=\"tabela\">
                        <tr>
                        <td class=\"SubTituloDireita\">Nome do Evento:</td>
                        <td class=\"SubTituloEsqueda\"><b>{$title}</b></td>
                        {$saldos}
                    </tr>
                    <tr>
                        <td class=\"SubTituloDireita\">Fiscal do Evento:</td>
                        <td class=\"SubTituloEsqueda\"><b>{$gestor}</b></td>
                    </tr>
    ";

    if (( $condicao != 1) AND ( $condicao != '')) {
        $cab.="
            <tr>
                <td class=\"SubTituloDireita\">Unidade Demandante:</td>
                <td class=\"SubTituloEsqueda\"><b>{$unidadeDemandante}</b></td>
            </tr>
        ";
    }

    if ($adreferendum == 't') {
        $refer = "<img src=\"/imagens/check.jpg\" border=0\">";
    } else {
        $refer = " -- ";
    }

    $cab .= "
                <tr>
                    <td class=\"SubTituloDireita\">AD Referendum:</td>
                    <td class=\"SubTituloEsqueda\"><b>{$refer}</b></td>
                </tr>
                <tr>
                    <td class=\"SubTituloDireita\">Data de Inclusão:</td>
                    <td class=\"SubTituloEsqueda\"><b>{$dtInclusao}</b></td>
                </tr>
            </table>
        </td>
    ";

    if ($valores) {
        $cab .= "
            <td>
                <table class=\"tabela\" width=\"100%\">
                    <tr>
                        <td class=\"SubTituloDireita\"> <b> Saldo Inicial do Empenho </b> </td>
                        <td>" . number_format($arrFinanceiroEvento['saldo_inicial'], 2, ",", ".") . "</td>
                    </tr>
                    <tr>
                        <td class=\"SubTituloDireita\"> <b> Valor Utilizado do Empenho: </b> </td>
                        <td>" . number_format($arrFinanceiroEvento['valor_utilisado'], 2, ",", ".") . "</td>
                    </tr>
                    <tr>
                        <td class=\"SubTituloDireita\"> <b> Custo Previsto do Evento:</b> </td>
                        <td>" . number_format($arrFinanceiroEvento['custo_prev'], 2, ",", ".") . "</td>
                    </tr>
                    <tr>
                        <td class=\"SubTituloCentro\" colspan=\"2\"> &nbsp; </td>
                    </tr>
                    <tr>
                        <td class=\"SubTituloCentro\" colspan=\"2\"> &nbsp; </td>
                    </tr>
                </table>
            </td>
        ";
    }
    $cab .= "
            </tr>
    </table>
    ";

    echo $cab;
}

function headCompra( $copnumprocesso, $codataabertura, $codsc ){
	$cab = "<table align=\"center\" class=\"Tabela\">
			 <tbody>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">N° do Processo</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$copnumprocesso}</td>
				</tr>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Data de Abertura</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$codataabertura}</td>
				</tr>";


	$cab.="<tr>
						<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Tipo de Cotação</td>
						<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$codsc}</td>
					</tr>";

	$cab.="	 </tbody>
			</table>";
	echo $cab;
}

function montaCabecalhoProcesso($copid, $mostraLink = true){

	global $db;

	if($copid){
		$sql = "SELECT p.copnumprocesso, p.copdsc,
					  to_char(p.copdatalimite,'dd/mm/YYYY') as copdatalimite,
					  cocdsc
				FROM evento.coprocesso p
					left join evento.cotipocotacao tc on p.cocid = tc.cocid
				WHERE p.copid = $copid";

		$dados = $db->pegaLinha($sql);
		$cab = '<table class="tabela" align="center" bgcolor="#f5f5f5" cellpadding="3" cellspacing="1">
	    	<tbody>
		    	<tr>
		        	<td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right">Número do Processo:</td>
		        	<td>
		        	';
		if($mostraLink){
			$cab .= '<a href="evento.php?modulo=principal/cadProcesso&acao=A">'. $dados['copnumprocesso'] .'</a>';
		} else {
			$cab .= $dados['copnumprocesso'];
		}
		$cab .= '</td>
		    	</tr>
				<tr>
			        <td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right">Tipo de Cotação:</td>
			        <td>'. $dados['cocdsc'] .'</td>
			    </tr>
			    <tr>
			        <td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right">Data limite para adesão:</td>
			        <td>'. $dados['copdatalimite'] .'</td>
			    </tr>
	    	</tbody>
		</table>';
	} else {
		$cab = "";
	}
	echo $cab;

}

function montaCabecalhoUnidade($usgid){

	global $db;

	if($usgid){
		$sql = "SELECT usgid, usgdsc, usgcod FROM evento.uasg WHERE usgid = $usgid";
		$dados = $db->pegaLinha($sql);
		$cab = '<table class="tabela" align="center" bgcolor="#f5f5f5" cellpadding="3" cellspacing="1">
	    	<tbody>
		    	<tr>
		        	<td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right">Unidade:</td>
		        	<td>'. $dados['usgcod'] .' - '.$dados['usgdsc'] .'</td>
		    	</tr>
	    	</tbody>
		</table>';

	} else {
		$cab = "";
	}
	echo $cab;

}
function montaCabecalhoContrato($ctrid, $mostraLink = true){

	global $db;

	if($ctrid){
		$sql = "SELECT tpc.tpcdsc || ' Nº ' ||  ctr.ctrnum || ' / ' || ctr.ctrano as numcontrato,
					   mod.moddsc as moddsc,
					   ctr.ctrobj as ctrobj
				FROM evento.ctcontrato ctr
					left join evento.ctmodalidadecontrato mod on ctr.modid = mod.modid
					left join evento.cttipocontrato tpc on ctr.tpcid = tpc.tpcid
				WHERE ctr.ctrid = $ctrid";

		$dados = $db->pegaLinha($sql);
		$cab = '<table class="tabela" align="center" bgcolor="#f5f5f5" cellpadding="3" cellspacing="1" style="border-bottom:2px solid #000;">
	    	<tbody>
		    	<tr>
		        	<td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right"><b>Número:</b></td>
		        	<td>
		        	';
		if($mostraLink){
			$cab .= '<a href="evento.php?modulo=principal/cadContrato&acao=A">'. $dados['numcontrato'] .'</a>';
		} else {
			$cab .= $dados['numcontrato'];
		}
		$cab .= '</td>
		    	</tr>
				<tr>
			        <td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right"><b>Tipo:</b></td>
			        <td>'. $dados['moddsc'] .'</td>
			    </tr>
			    <tr>
			        <td class="SubTituloDireita" style="vertical-align: top; width: 25%;" align="right"><b>Objeto:</b></td>
			        <td>'. $dados['ctrobj'] .'</td>
			    </tr>
	    	</tbody>
		</table>';
	} else {
		$cab = "";
	}
	echo $cab;

}


/**
 * @author: Pedro Dantas
 * @date: 18/02/2009
 * @params: no
 * @returns: boolean
 * @coments: verifica se os eventos já cadastrados deste usuario tem notas técnicaas
 * 			 caso ja tenha 2 eventos cadastrados sem nota técnica a função retorna 'false'
 */
function verificaEventos( $ungcod ){
	global $db;

//	$sqlEvePassados = "select
//						evedatafim ,
//						eveid,
//						evedatafim - integer '10'
//						from evento.evento
//						where
//						evedatafim < date(now()) - integer '10' and
//						evedatafim - integer '10' < now()
//						and evestatus = 'A'
//						and ungcod = '".$ungcod."'
//						order by evedatafim ";

	$sqlEvePassados = "
							select
						 *,
						e.evedatafim - integer '10'
						from evento.evento e
						inner join workflow.documento as d on d.docid = e.docid
						inner join workflow.estadodocumento as es on es.esdid = d.esdid
						where
						e.evedatafim < date(now()) - integer '10' and
						e.evedatafim - integer '10' < now()
						and e.evestatus = 'A'
						and ungcod = '".$ungcod."'
						and es.esdid <> ".CADASTRAMENTO_WF."
						order by e.evedatafim ";

	$arrEventos = $db->carregar( $sqlEvePassados );
 	$arrSemNota = array();
 	$arrSemAval = array();
 	$arrTemNota = array();
 	$rsSemNota  = array();
 	$pend = 0;

	for( $i = 0; $i < count( $arrEventos ); $i++){
		if( $arrEventos[$i]['eveid']!= '' ){
			$sqlBuscaNota = "SELECT distinct tpaid FROM evento.anexoevento where eveid =  ".$arrEventos[$i]['eveid']." and tpaid = 1 and axestatus = 'A'";
			$tpaid        = $db->pegaUm( $sqlBuscaNota );
			if( !$tpaid ){
				array_push( $arrSemNota , 'sem_nota' );
			}
			$sqlBuscaAval = "select e.eveid from evento.evento as e inner join evento.avaliacaoevento as a on a.eveid = e.eveid where e.eveid = ".$arrEventos[$i]['eveid'];
			$aval = $db->pegaUm( $sqlBuscaAval );
			if( !$aval){
				array_push( $arrSemAval , 'sem_aval' );
			}
		}
	}
	//$numEventosSemNota = count( $arrSemNota );
	$numEventosSemNota = 0;
	$numEventosSemAval = count( $arrSemAval );
	return $numEventosSemNota.'_'.$numEventosSemAval;
}

function existeAvaliacao(){
	global $db;
	$sql = "select e.eveid from evento.evento as e inner join evento.avaliacaoevento as a on a.eveid = e.eveid
			where e.eveid = ".$_SESSION['evento']['eveid'];
	if( $_SESSION['evento']['eveid'] ){
		$evento = $db->pegaUm( $sql );

		if( $evento ) {
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

function getUnidadeByCpf( $eveid = false ){
	global $db;
	if( !$eveid ){
		$sql = "SELECT ungcod FROM seguranca.usuario WHERE usucpf = '{$_SESSION['usucpf']}'";
	}else{
		$sql = "SELECT ungcod FROM evento.evento WHERE eveid = ".$_SESSION['evento']['eveid'];
	}
	$cod = $db->pegaUm( $sql );
	return $cod;
}

    function validaTramit(){
        global $db;

        $sql = "
            SELECT DISTINCT e.eveid,
                    e.evetitulo,
                    ta.tpaid,
                    unr.urevalorrecurso,
                    e.evecustoprevisto

            FROM evento.evento AS e

            INNER JOIN evento.itemevento AS itm ON itm.eveid = e.eveid
            INNER JOIN evento.anexoevento AS anx ON anx.eveid = e.eveid
            INNER JOIN evento.tipoanexo AS ta ON ta.tpaid = anx.tpaid
            INNER JOIN evento.unidaderecurso_old AS unr ON unr.ureid = e.ureid

            WHERE e.evestatus ='A' AND ta.tpaid = 2 AND anx.axestatus = 'A'
                    AND e.evenumeropi IS NOT NULL
                    AND e.eveanopi IS NOT NULL
                    AND unr.urevalorrecurso IS NOT NULL
                    AND  e.eveid = {$_SESSION['evento']['eveid']}
        ";
        $rs = $db->pegaLinha( $sql );

        if( $rs ){
            if($rs['urevalorrecurso'] >= $rs['evecustoprevisto']){
                return true;
            }
        }
        return false;
    }

function pre( $var1, $die = false )
{
	if( $var1 != '' )
	{
		echo("<pre>");
			   	print_r( $var1 );
		echo("</pre>");
	}
	if( $die == 1 )
		die();
}

/**
 * Recupera o docid vinculado ao evento
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param (int|null) $eveid Se for null, assumirá o valor da SESSION['eveid']
 * @return (int|null) docid
 */
function evtPegarDoc($eveid=null){
	global $db;

	$eveid = $eveid ? $eveid : $_SESSION['evento']['eveid'];

	$sql = "SELECT
				docid
			FROM
				evento.evento
			WHERE
				eveid = {$eveid}";

	return $db->pegaUm($sql);
}

/**
 * Inseri o evento no documento, fazendo com o mesmo entre do Workflow.
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param (int|null) $eveid Se for null, assumirá o valor da SESSION['eveid']
 * @return (int) docid
 */
function evtCriarDoc($eveid=null){
	global $db;

	$eveid = $eveid ? $eveid : $_SESSION['evento']['eveid'];

	if (!$eveid)
		return false;

	$docid = evtPegarDoc($eveid);

	if (!$docid){
		/*
		 * Pega tipo do documento "WORKFLOW"
		 */
		$sql = "SELECT
					tpdid
				FROM
					workflow.tipodocumento
				WHERE
					tpdid =".WF_TPDID_EVENTOS;

		$tpdid = $db->pegaUm($sql);
		/*
		 * Pega nome do evento
		 */
		$sql = "SELECT
					evetitulo
				FROM
					evento.evento
				WHERE
					eveid ={$eveid}";

		$tit = $db->pegaUm($sql);

		$docdsc = "Cadastramento Evento - " . $tit;
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
		/*
		 * Atualiza o $docid no evento
		 */
		$sql = "UPDATE evento.evento SET
					docid = '".$docid."'
				WHERE
					eveid = {$eveid}";

		$db->executar( $sql );
		$db->commit();
	}

	return $docid;
}
function arrayPerfil(){
	global $db;

	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = 21
					WHERE
					 pu.usucpf = '%s'
					ORDER BY
					 p.pflnivel",
				$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}

function montaBradScrum( $arrLinks ){

	if( is_array( $arrLinks ) ) {

		echo("<table width=\"100%\" align=\"center\"class=\"tabela\">");
		echo("<tr>");
		echo("<td>");
			for($i = 0; $i<count( $arrLinks ); $i++) {

				$texto = $arrLinks[$i]['texto'];
				$link  = $arrLinks[$i]['link'];

				$content .= " <img align=\"absmiddle\" src=\"/imagens/arrow_h.png\" /> <a href=\"$link\"> $texto </a>";
			}
			echo("<b>Você está em:</b> $content");
		echo("</td>");
		echo("</tr>");
		echo("</table>");
	}
}


    function enviarEmailConfirm(){
	global $db;

	$sql = "
            SELECT  evetitulo,
                    to_char(evedatainicio::date,'DD/MM/YYYY') as evedatainicio,
                    to_char(evedatafim::date,'DD/MM/YYYY') as evedatafim,
                    ureid,
                    evecustoprevisto
            FROM evento.evento

            WHERE eveid = '{$_SESSION['evento']['eveid']}'
        ";
	$rs = $db->carregar( $sql );

	$arrEmails = array($_SESSION['email_sistema']);
	$remetente = array('nome'=>REMETENTE_WORKFLOW_NOME, 'email'=>REMETENTE_WORKFLOW_EMAIL);

	$assunto   = "[SIMEC] Novo Evento cadastrado no SIMEC - Módulo de Eventos";
	$mailBody = '
            Prezados Senhores, <br><br>
            Informamos que o evento nº '.$_SESSION['evento']['eveid'].' - "'.$rs[0]['evetitulo'].'" a ser realizado no período de '.$rs[0]['evedatainicio'].' à '.$rs[0]['evedatafim'].',<br>
            foi cadastrado no SIMEC e enviado para análise e aprovação do comitê de eventos.<br>
            <br><br>
            <a href="http://simec.mec.gov.br">Clique Aqui para acessar o SIMEC.</a>
            <br><br>
            Atenciosamente, <br><br><br>
            '.$remetente['nome'].'<br>
	';

	atulaizarSaldoEnvio();

	if($_REQUEST['esdid'] == EM_ANALISE_COMITE_WF){
            if(verificaPrazoConformeComite()){
                enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
                return true;
            }else{
                return false;
            }
	}else{
            enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
            return true;
	}
    }

/*
function eventoPosAcaoAssinadoSaa(){
	global $db;

	$sql = "SELECT
				evetitulo,
				to_char(evedatainicio::date,'DD/MM/YYYY') as evedatainicio,
				to_char(evedatafim::date,'DD/MM/YYYY') as evedatafim,
				eveemail,
				evecustoprevisto
			FROM
				evento.evento
			WHERE
				eveid = '{$_SESSION['evento']['eveid']}'";

	$rs = $db->carregar( $sql );

	$destinatario = $rs[0]['eveemail'];

	$remetente = array('nome'=>REMETENTE_WORKFLOW_NOME, 'email'=>REMETENTE_WORKFLOW_EMAIL);

	$assunto   = "[SIMEC] Evento Assinado pelo SAA - Módulo de Eventos";
	$mailBody = '
	Prezado(s) Senhor(es), <br>
	<br>
	Informamos que o evento nº '.$_SESSION['evento']['eveid'].' - "'.$rs[0]['evetitulo'].'" a ser realizado no período de '.$rs[0]['evedatainicio'].' à '.$rs[0]['evedatafim'].',<br>
	foi Assinado pelo SAA. É necessário o preenchimento da ficha de avaliação.<br>
	<br>
	<br>
	<a href="http://simec.mec.gov.br">Clique Aqui para acessar o SIMEC.</a>
	<br>
	<br>
	Atenciosamente,<br>
	<br>
	<br>
	'.$remetente['nome'].'<br>
	';

	enviar_email($remetente, $destinatario, $assunto, $mailBody, $mailCopia );
	return true;

}
*/

    function eventoPosAcaoGerarOS(){
	global $db;

        $eveid = $_SESSION['evento']['eveid'];

        $evedatainclusao = buscaDataInclusaoEvento();
        $data_nova_regra_evento = strtotime(DATA_NOVO_REGRA_EVENTO_NOVA_INFRA);

        if(!$eveid){
            return false;
        }

        #GERA NUMERO O.S.
        $sql = "SELECT (COALESCE(max(gnosequencial),0)+1) AS total FROM evento.geranumeroos";
        $seqos = $db->pegaUm($sql);

        #INSERI O NÚMERO DO OS.
        $sql = "INSERT INTO evento.geranumeroos(gnosequencial) VALUES ($seqos)";
        $db->executar($sql);


        #BUSCA DADOS PREGAO.
        if( $evedatainclusao < $data_nova_regra_evento ){
            #BUSCA DADOS PREGAO - LEGADO.
            $sql = "SELECT precodpregao, precnpj, prerazaosocial, prenumcontrato FROM evento.pregaoevento_old WHERE prestatus='A' AND CURRENT_DATE BETWEEN preiniciovig AND prefimvig LIMIT 1";
            $dadosPregao = $db->pegaLinha($sql);
        }else{
            #BUSCA DADOS PREGAO - ATUAL.
            $sql = "
                SELECT 	p.precodpregao AS precodpregao,
                        c.coecnpj AS precnpj,
                        coerazaosocial AS prerazaosocial,
                        coenumcontrato AS prenumcontrato
                FROM evento.evento AS e

                JOIN evento.empenho_unidade AS eu ON eu.emuid = e.emuid
                JOIN evento.contratopregao AS c ON c.coeid = eu.coeid
                JOIN evento.pregaoevento AS p ON p.preid = c.preid

                WHERE eveid = {$eveid};
            ";
            $dadosPregao = $db->pegaLinha($sql);
        }

        #BUSCA DADOS EVENTO
        $sql = "SELECT evenumeroprocesso, evedatainicio, evedatafim, evecustoprevisto FROM evento.evento WHERE eveid = {$eveid}";
        $dadosEvento = $db->pegaLinha($sql);

        #INSERE O.S.
        $sql = "
            INSERT INTO evento.ordemservico(
                    eveid, osenumeroos, osedataemissaoos, osedatainiciofinal, osedatafimfinal, osecustofinal, oseobsos, osecnpj,
                    oserazaosocial, oseproposta, osecodpregao, oseordenador, oseempenho, osenumcontrato, oseststus
                )VALUES (
                    {$eveid}, {$seqos}, 'NOW()', '{$dadosEvento['evedatainicio']}', '{$dadosEvento['evedatafim']}', NULL, NULL, '{$dadosPregao['precnpj']}',
                    '{$dadosPregao['prerazaosocial']}', NULL, '{$dadosPregao['precodpregao']}', NULL, NULL, '{$dadosPregao['prenumcontrato']}', 'A'
                ) RETURNING oseid;
        ";
        $oseid = $db->pegaUm($sql);

        if( $oseid > 0 ){
            $db->commit();
            return true;
        }else{
            return false;
        }
    }

    function eventoEnviarPagamento(){
	global $db;

	//verifica perfil DRP
	/*
	if( pegaPerfil($_SESSION['usucpf']) != EVENTO_PERFIL_DRP && pegaPerfil($_SESSION['usucpf']) != EVENTO_PERFIL_SUPER_USUARIO ){
            return 'É necessário possuir o perfil DRP!';
	}
	*/

	#VERIFICA SE PREENCHEU A AVALIAÇÃO (ITENS RADIO)
	$verificaAvaliacao = true;

	$sql = "select aquid, aqudescricao from evento.assuntoquestao order by aquid ";
	$rsAssunto = $db->carregar( $sql );

	for( $i = 0; $i< count($rsAssunto); $i++){
            $sql = "
                SELECT q.qavid, q.qevdescricao, tq.tqadescricao
		FROM evento.questaoavaliacao AS q
		INNER JOIN evento.tipoquestaoavaliacao AS tq ON q.tqaid = tq.tqaid
                WHERE q.qevstatus = 'A' AND q.aquid = '{$rsAssunto[$i]['aquid']}'
            ";
            $rsQestaoAvaliacao = $db->carregar( $sql );

            for( $j = 0; $j < count( $rsQestaoAvaliacao ); $j++ ){
           	$qavid = $rsQestaoAvaliacao[$j]['qavid'];
                $sql = "SELECT count(eavid) FROM evento.avaliacaoevento WHERE eveid = {$_SESSION['evento']['eveid']} AND qavid = {$qavid}";
                $tem = $db->pegaUm( $sql );

                if( $tem == 0 ){
                    $verificaAvaliacao = false;
                }
	   }
	}

	#VERIFICA SE PREENCHEU A AVALIAÇÃO (ITENS TEXTAREA)
	$sqlQSub = "SELECT * FROM evento.questaosubjetivaevento";
	$rsQSub = $db->carregar( $sqlQSub );
	for( $s = 0; $s <count( $rsQSub ); $s++ ){
            $sqlResp  = "
                SELECT r.rasresposta
                FROM evento.avaliacaosubjetivaevento AS a
                INNER JOIN evento.respostaavaliacaosubjetivaeve AS r ON a.rasid = r.rasid
                WHERE a.eveid = {$_SESSION['evento']['eveid']} AND a.qusid = {$rsQSub[$s]['qusid']}
            ";
            $rasresposta = $db->pegaUm( $sqlResp );

            if( !$rasresposta ){
                $verificaAvaliacao = false;
            }
	}
	if($verificaAvaliacao == false){
            return "É necessário preencher toda a ficha de avaliação!";
	}
	return true;
    }


function eventoRegistrarPagamento(){
	global $db;

	if(!$_SESSION['evento']['eveid']) return 'Sessão expirou. Entre novamente no sistema.';

 	$sql = "SELECT count(dpaid) FROM evento.documentopagamento WHERE eveid = ".$_SESSION['evento']['eveid'];
	$tem = $db->pegaUm( $sql );
	if( $tem == 0 ){
		return false;
	}

	return true;
}


    function atulaizarSaldoEnvio(){
        global $db;

        $eveid  = $_SESSION['evento']['eveid'];
        $usucpf = $_SESSION['usucpf'];

        $evedatainclusao = buscaDataInclusaoEvento();
        $data_nova_regra_evento = strtotime(DATA_NOVO_REGRA_EVENTO_NOVA_INFRA);

        $sql = "
            SELECT  ureid,
                    emuid,
                    evetitulo,
                    evecustoprevisto
            FROM evento.evento
            WHERE eveid = {$eveid}
        ";
        $dados = $db->pegaLinha( $sql );

        $emuid = $dados['emuid'];

        if( $evedatainclusao >= $data_nova_regra_evento && $emuid > 0 ){
            $SQL = "SELECT ureid, coeid, empvalorutilizado FROM evento.empenho_unidade WHERE emuid = {$emuid}";
            $data = $db->pegaLinha($SQL);
            $ureid = $data['ureid'];
            $coeid = $data['coeid'];
        }else{
            $ureid = $dados['ureid'];
        }
        $evetitulo          = addslashes( $dados['evetitulo'] );
        $evecustoprevisto   = $dados['evecustoprevisto'];

        if( $ureid > 0 ){

            if( $evedatainclusao >= $data_nova_regra_evento && $emuid > 0 ){
                #CONTA CORRENTE.
                $sql = "
                    INSERT INTO evento.unidadecontacorrente(
                            ureidpai, eveid, uccdesclancamento, uccvalorlancamento, uccdatalancamento, ucccpf
                        )VALUES(
                            {$ureid}, {$eveid}, '{$evetitulo}', '{$evecustoprevisto}', 'NOW()', '{$usucpf}'
                    );
                ";
                $db->pegaUm($sql);

                #INICIO: ATUALIZA - EMPENHO
                #BUSCA VALOR COMSUMIDO NO EMPENHO E SOMA A ELE O VALOR PREVISTO DO EVENTO, GERANDO O VALOR UTILIZDO DO EMPENHO.
                $ev_valor_utilizado = $evecustoprevisto + $data['empvalorutilizado'];

                #ATUALIZA O EMPENHO COM O VALOR UTILIZADO.
                $sql = "UPDATE evento.empenho_unidade SET empvalorutilizado = '{$ev_valor_utilizado}' WHERE emuid = {$emuid} AND empstatus = 'A' RETURNING emuid;";
                $db->pegaUm($sql);
                #FIM: ATUALIZA - EMPENHO

                #INICIO: ATUALIZA - UNIDADE
                #BUSCA VALOR UTILIZADO PELO EMPENHO, QUE É A SOMA DOS VALORES UTILIZADOS NOS EMEPNHOS PELA UNIDADE.
                $sql = "SELECT sum(empvalorutilizado) AS empvalorutilizado FROM evento.empenho_unidade WHERE ureid = {$ureid};";
                $ep_valor_utilizado = $db->pegaUm($sql);

                #ATUALIZA A UNIDADE COM O VALOR UTILIZADO.
                $sql = "UPDATE evento.unidaderecurso SET urevalorutilizado = '{$ep_valor_utilizado}' WHERE ureid = {$ureid} RETURNING ureid;";
                $db->pegaUm($sql);
                #FIM: ATUALIZA - UNIDADE

                #INICIO: ATUALIZA - CONTRATO
                #BUSCA VALOR UTILIZADO PELA UNIDADE, QUE É A SOMA DO VALOR UTILIZADO NA UNIDADE.
                $sql = "SELECT sum(urevalorutilizado) AS urevalorutilizado FROM evento.unidaderecurso WHERE coeid = {$coeid};";
                $co_valor_utilizado = $db->pegaUm($sql);

                #ATUALIZA O CONTRATO COM O VALOR UTILIZADO.
                $sql = "UPDATE evento.contratopregao SET coevalorutilizado = '{$co_valor_utilizado}' WHERE coeid = {$coeid} RETURNING coeid;";
                $up_saldo = $db->pegaUm($sql);
                #FIM: ATUALIZA - CONTRATO

            }else{
                #CONTA CORRENTE.
                $sql = "
                    INSERT INTO evento.unidadecontacorrente_old(
                            ureidpai, eveid, uccdesclancamento, uccvalorlancamento, uccdatalancamento, ucccpf
                        )VALUES(
                            {$ureid}, {$eveid}, '{$evetitulo}', '{$evecustoprevisto}', 'NOW()', '{$usucpf}'
                    );
                ";
                $db->pegaUm($sql);

                $sql = "UPDATE evento.unidaderecurso_old SET urevalorsaldo = urevalorsaldo - {$evecustoprevisto} where ureid = {$ureid} RETURNING ureid;";
                $up_saldo = $db->pegaUm($sql);
            }
        }

        if( $up_saldo > 0 ){
            $db->commit();
            return true;
        }
        return false;
    }

    function atualizarSaldoVoltarUnidade(){
	global $db;

        $eveid  = $_SESSION['evento']['eveid'];
        $usucpf = $_SESSION['usucpf'];

        $evedatainclusao = buscaDataInclusaoEvento();
        $data_nova_regra_evento = strtotime(DATA_NOVO_REGRA_EVENTO_NOVA_INFRA);

        $sql = "
            SELECT  ureid,
                    emuid,
                    evetitulo,
                    evecustoprevisto
            FROM evento.evento
            WHERE eveid = {$eveid}
        ";
        $dados = $db->pegaLinha( $sql );

        $emuid = $dados['emuid'];

        if( $evedatainclusao >= $data_nova_regra_evento && $emuid > 0 ){
            $SQL = "SELECT ureid, coeid, empvalorutilizado FROM evento.empenho_unidade WHERE emuid = {$emuid}";
            $data = $db->pegaLinha($SQL);
            $ureid = $data['ureid'];
            $coeid = $data['coeid'];
        }else{
            $ureid = $dados['ureid'];
        }
        $evetitulo          = addslashes( $dados['evetitulo'] );
        $evecustoprevisto   = $dados['evecustoprevisto'];

        if( $ureid > 0 ){

            if( $evedatainclusao >= $data_nova_regra_evento && $emuid > 0 ){
                #CONTA CORRENTE.
                $sql = "
                    INSERT INTO evento.unidadecontacorrente(
                            ureidpai, eveid, uccdesclancamento, uccvalorlancamento, uccdatalancamento, ucccpf
                        )VALUES(
                            {$ureid}, {$eveid}, 'Estorno', '{$evecustoprevisto}', 'NOW()', '{$usucpf}'
                    );
                ";
                $db->pegaUm($sql);

                #INICIO: ATUALIZA - EMPENHO
                #BUSCA VALOR COMSUMIDO NO EMPENHO E SUBTRAI DELE O VALOR PREVISTO DO EVENTO, GERANDO O VALOR UTILIZADO DO EMPENHO.
                $ev_valor_utilizado = $data['empvalorutilizado'] - $evecustoprevisto;

                #ATUALIZA O EMPENHO COM O VALOR UTILIZADO.
                $sql = "UPDATE evento.empenho_unidade SET empvalorutilizado = '{$ev_valor_utilizado}' WHERE emuid = {$emuid} AND empstatus = 'A' RETURNING emuid;";
                $db->pegaUm($sql);
                #FIM: ATUALIZA - EMPENHO

                #INICIO: ATUALIZA - UNIDADE
                #BUSCA VALOR UTILIZADO PELO EMPENHO, QUE É A SOMA DO VALOR UTILIZADO NO EMEPNHO PELA UNIDADE.
                $sql = "SELECT sum(empvalorutilizado) AS empvalorutilizado FROM evento.empenho_unidade WHERE ureid = {$ureid};";
                $ep_valor_utilizado = $db->pegaUm($sql);

                #ATUALIZA O EMPENHO COM O VALOR UTILIZADO.
                $sql = "UPDATE evento.unidaderecurso SET urevalorutilizado = '{$ep_valor_utilizado}' WHERE ureid = {$ureid} RETURNING ureid;";
                $db->pegaUm($sql);
                #FIM: ATUALIZA - UNIDADE

                #INICIO: ATUALIZA - CONTRATO
                #BUSCA VALOR UTILIZADO PELA UNIDADE, QUE É A SOMA DO VALOR UTILIZADO NA UNIDADE.
                $sql = "SELECT sum(urevalorutilizado) AS urevalorutilizado FROM evento.unidaderecurso WHERE coeid = {$coeid};";
                $co_valor_utilizado = $db->pegaUm($sql);

                #ATUALIZA O CONTRATO COM O VALOR UTILIZADO.
                $sql = "UPDATE evento.contratopregao SET coevalorutilizado = '{$co_valor_utilizado}' WHERE coeid = {$coeid} RETURNING coeid;";
                $up_saldo = $db->pegaUm($sql);
                #FIM: ATUALIZA - CONTRATO

            }else{
                $sql = "
                    INSERT INTO evento.unidadecontacorrente_old(
                            ureidpai, eveid, uccdesclancamento, uccvalorlancamento, uccdatalancamento, ucccpf
                        )VALUES(
                            {$ureid}, {$eveid}, 'Estorno', '{$evecustoprevisto}', 'NOW()', '{$usucpf}'
                    );
                ";
                $db->pegaUm($sql);

                $sql = "UPDATE evento.unidaderecurso_old  SET urevalorsaldo = urevalorsaldo + {$evecustoprevisto} WHERE ureid = {$ureid} RETURNING ureid;";;
                $up_saldo = $db->pegaUm($sql);
            }
        }

        if( $up_saldo > 0 ){
            $db->commit();
            return true;
        }
        return false;
    }

function aprovarEvento(){
	global $db;
	if( $_SESSION['evento']['eveid'] != '' ){
		$id = $_SESSION['evento']['eveid'];
		$sql = "UPDATE evento.evento SET sevid = 3 WHERE eveid = $id ";
		$up = $db->executar( $sql );
		$db->commit();
	}
}
 function listaSituacaoPorUF($id = "tabela_1",$sql,$titulo = null,$cabecalho = null,$sqlAgrupador = array(),$exibeSoma = "N",$link = array(),$arrOff = array()){
	 global $db;
	 $dados = $db->carregar($sql);

	 $tabela = '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';


	 if(!$dados){
	 	$tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
	 	echo $tabela;
	 	return false;
	 }

	 $num_colunas = count($dados[0]);
	 $num_colunas = $num_colunas - (count($arrOff));

	 if($titulo){
	 	$tabela .= "<tr bgcolor=#CCCCCC ><td colspan=\"$num_colunas\" align=center ><b>$titulo</b></td></tr>";
	 }

	 if($cabecalho){
	 	$tabela .= "<tr bgcolor=#e9e9e9 onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#e9e9e9'\" >";
		 $i = 0;
		 while($i < $num_colunas){
		 	$tabela .= "<td><b>".$cabecalho[$i]."</b></td>";
		 	$i++;
		 }
		 $tabela .= "</tr>";
	 }
	 $id_span = 1;
	 $i = 0;
	 foreach($dados as $d){
	 	($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC";

	 	$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";

	 	$sqlAg = $sqlAgrupador['sql'];

	 	if($sqlAgrupador['sql']){
	 		if($sqlAgrupador['agrupador'] && $d[$sqlAgrupador['agrupador']]){
	 			$sqlAg = str_replace("|agrupador|",$d[$sqlAgrupador['agrupador']],$sqlAg);
	 			$dadosAgrupados = $db->carregar($sqlAg);
	 		}else{
	 			$dadosAgrupados = "";
	 		}
	 		$listaAgrupada = '<table cellspacing="0" cellpadding="2" border="0" align="center" width="100%" class="listagem">';

	 		if(!$dadosAgrupados){
	 			$listaAgrupada .= "<tr><td><span style=\"color:#990000\" >Não existem registros.</span></td></tr>";
	 		}else{

	 			$xx = 0;
	 			foreach($dadosAgrupados as $dA){
	 				($xx % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC";
	 				$listaAgrupada .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";

	 				foreach($dA as $k => $dd){
	 					$kk[] = $k;
	 				}
	 				$ii = 0;
	 				while($ii < count($dA)){

	 					if($sqlAgrupador['link']){
	 						if($sqlAgrupador['campo']){
	 							if(is_array($sqlAgrupador['campo'])){
	 								unset($arrCampos);
	 								foreach($sqlAgrupador['get'] as $cmp){
	 									$arrCampos[] = "{$cmp}={$dA[$cmp]}";
	 									$campos = implode("&",$arrCampos);
	 								}
	 							}else{
	 								$campos = "{$sqlAgrupador['get']}={{$dA[$kk[$sqlAgrupador['get']]]}}";
	 							}
	 						}

	 						$linkAg_a = "<a href=\"".$sqlAgrupador['link']."&".$campos."\" />";
	 						$linkAg_b = " </a>";
	 					}

	 					if($kk[$ii] == $kk[0]){
	 						$seta_filho = "<img src=\"../imagens/seta_filho.gif\" />";
	 					}else{
	 						$seta_filho = "";
	 					}

	 					if(!strstr($kk[$ii],"id") && !strstr($kk[$ii],"ordem") && !in_array($kk[$ii],$sqlAgrupador['arrOff'])){

	 						if(in_array($kk[$ii],$sqlAgrupador['exibeLink'])){

			 					if(is_numeric($dA[$kk[$ii]])){
							 		$campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
							 		$listaAgrupada .= "<td align=\"right\"><span style=\"color:rgb(0, 102, 204);text-align:right\" >$seta_filho $linkAg_a $campo $linkAg_b</span></td>";
			 					}
							 	else{
							 		if( $dA[$kk[$ii]] == '' ){
							 			$dA[$kk[$ii]] = "sem estado cadastrado";
							 			$linkAg_a	  = "";
							 			$linkAg_b	  = "";
							 		}
							 		$listaAgrupada .= "<td>$seta_filho $linkAg_a {$dA[$kk[$ii]]} $linkAg_b</td>";
							 	}
	 						}
	 						else{
	 							if(is_numeric($dA[$kk[$ii]])){
							 		$campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
							 		$listaAgrupada .= "<td align=\"right\" ><span style=\"color:rgb(0, 102, 204);text-align:right;width:100%\" >$seta_filho $campo</span></td>";
			 					}
							 	else{
							 		$listaAgrupada .= "<td>$seta_filho {$dA[$kk[$ii]]}</td>";
							 	}
	 						}
	 					}
						$ii++;

	 				}
	 				$listaAgrupada .= "</tr>";
	 			$xx++;
	 			}
	 		}
	 		$listaAgrupada .= "</table>";
	 	}

	 	$keys = array_keys($d);
	 	$j = 0;
		while($j < $num_colunas){
			if($sqlAgrupador && $keys[$j] == $keys[0] && $dadosAgrupados){
				$img = "<img onclick=\"exibeAgrupador('{$id}_{$id_span}')\" style=\"cursor:pointer\" id=\"img_mais_{$id}_{$id_span}\" align=\"abdmiddle\" src=\"../imagens/mais.gif\" title=\"Abrir\" />
						<img onclick=\"escondeAgrupador('{$id}_{$id_span}')\" style=\"cursor:pointer;display:none\" id=\"img_menos_{$id}_{$id_span}\" align=\"abdmiddle\" src=\"../imagens/menos.gif\" title=\"Fechar\" /> ";
				$span = "<tr style=\"display:none\" bgcolor='#EEE9E9' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#EEE9E9'\" id=\"tr_view_{$id}_{$id_span}\"><td colspan=\"$num_colunas\">$listaAgrupada</td></td></tr>";
				$id_span ++;
			}
			else{
				$img = "&nbsp;&nbsp;&nbsp;&nbsp;";
			}

			//Monta os links;
			if($link && $dadosAgrupados){
				$link_a = "<a href=\"{$link['link']}&{$link['get']}=".$d[$link['get']]."\" >";
				$link_b = "</a>";
			}else{
				$link_a = "";
				$link_b = "";
			}


			if(!strstr($keys[$j],"id") && !strstr($keys[$j],"ordem") && !in_array($keys[$j],$arrOff)){

				if(is_numeric($d[$keys[$j]])){
					$tabela .= "<td align=\"right\">";
				}else{
					$tabela .= "<td>";
				}

				if($link['campo'] == $keys[$j]){
					$tabela .= $img.$link_a;
				}else{
					$tabela .= $img;
				}
			 	if(is_numeric($d[$keys[$j]])){
			 		$campo = str_replace(",",".",number_format($d[$keys[$j]]));
			 		$tabela .= "<span style=\"color:rgb(0, 102, 204)\" >".$campo.$link_b."</span></td>";
			 	}else{
				 	if($link['campo'] == $keys[$j]){
						$tabela .= $d[$keys[$j]].$link_b."</td>";
					}else{
						$tabela .= $d[$keys[$j]]."</td>";
					}

			 	}

			}

		 	if(!strstr($keys[$j],"ordem") && is_numeric($d[$keys[$j]])  && !in_array($keys[$j],$arrOff)){
		 		$soma[$keys[$j]] += $d[$keys[$j]];
		 		$campo_soma[] = $keys[$j];
		 	}
		 	$j++;

		}

	 	$tabela .= "</tr>";
	 	$tabela .= $span;

	 	$i++;
	 }

	 foreach($keys as $k => $k1){
	 	 if(strstr($k1,"id")){
	 	 	unset ($keys[$k]);
	 	 }
	 }

	 //Exibe Soma
	 if($exibeSoma == "S"){
	 	$tabela .= "<tr bgcolor='DCDCDC' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='DCDCDC'\" >";
	 	$campo_soma = array_unique($campo_soma);
	 	foreach($keys as $k1 => $k){

	 		if(!in_array($k,$arrOff)){

		 		if(in_array($k,$campo_soma)){
		 			$tabela .= "<td align=\"right\" ><b>".str_replace(",",".",number_format($soma[$k]))."</b></td>";
		 		}elseif($k1 == 0){
		 			$tabela .= "<td><b>Total:</b></td>";
		 		}else{
		 			$tabela .= "<td></td>";
		 		}
	 		}
	 	}
	 	$tabela .= "</tr>";
	 }

	 $tabela .= "</table>";
	 $tabela .="<script>
	 function exibeAgrupador(id){
	 	var img_mais = document.getElementById('img_mais_' +id);
	 	var img_menos = document.getElementById('img_menos_' +id);
	 	var tr_view = document.getElementById('tr_view_' +id);

	 	img_mais.style.display = 'none';
	 	img_menos.style.display = '';
	 	tr_view.style.display = '';

	 }

	 function escondeAgrupador(id){
	 	var img_mais = document.getElementById('img_mais_' +id);
	 	var img_menos = document.getElementById('img_menos_' +id);
	 	var tr_view = document.getElementById('tr_view_' +id);

	 	img_mais.style.display = '';
	 	img_menos.style.display = 'none';
	 	tr_view.style.display = 'none';

	 }

	 			</script>";

	 echo $tabela;
}

/**
 * Listar as entidadades e seus itens.
 *
 * @author Juliano Meinen de Souza
 * @param (int,sql,string,array,array,string,array,array)
 * @return (string) lista de unidades
 */
/*Função para montar lista com Agrupador e Links*/

/*
 * PEDRO DANTAS, FAVOR NÃO APAGAR ESSA FUNÇÃO!
 * R) - JULIANO, NÃO CRIPTOGRAFA AS FUNCOES!
 *
 */

function listaUnidadesLink($id = "tabela_1",$sql,$titulo = null,$cabecalho = null,$sqlAgrupador = array(),$exibeSoma = "N",$link = array(),$arrOff = array()){
     global $db;
     $dados = $db->carregar($sql);

     $tabela = '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';


     if(!$dados){
         $tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
         echo $tabela;
         return false;
     }

     $num_colunas = count($dados[0]);
     $num_colunas2 = count($dados[0]);
     $num_colunas = $num_colunas - (count($arrOff));

     foreach($dados[0] as $kkk => $ddd){
         if(strstr($kkk,"id") || strstr($kkk,"ordem")){
             $num_colunas2 --;
         }
     }

     $num_colunas2 = $num_colunas2 - (count($arrOff));

     if($titulo){
         $tabela .= "<tr bgcolor=#CCCCCC ><td colspan=\"$num_colunas2\" align=center ><b>$titulo</b></td></tr>";
     }

     if($cabecalho){
         $tabela .= "<tr bgcolor=#e9e9e9 onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#e9e9e9'\" >";
         $i = 0;
         while($i < $num_colunas2){
             $tabela .= "<td style=\"text-align:center\" ><b>".$cabecalho[$i]."</b></td>";
             $i++;
         }
         $tabela .= "</tr>";
     }
     $id_span = 1;
     $i = 0;
     foreach($dados as $d){
         ($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC";

         $tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";

         $sqlAg = $sqlAgrupador['sql'];

         if($sqlAgrupador['sql']){
             if($sqlAgrupador['agrupador'] && $d[$sqlAgrupador['agrupador']]){
                 $sqlAg = str_replace("|agrupador|",$d[$sqlAgrupador['agrupador']],$sqlAg);
                 $dadosAgrupados = $db->carregar($sqlAg);
             }else{
                 $dadosAgrupados = "";
             }
             $listaAgrupada = '<table cellspacing="0" cellpadding="2" border="0" align="center" width="100%" class="listagem">';

             if(!$dadosAgrupados){
                 $listaAgrupada .= "<tr><td><span style=\"color:#990000\" >Não existem registros.</span></td></tr>";
             }else{

                 if(is_array($sqlAgrupador['cabecalho'])){
                     $listaAgrupada .= "<tr bgcolor=#e9e9e9 onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#e9e9e9'\" >";
                     foreach($sqlAgrupador['cabecalho'] as $agCabecalho){
                         $listaAgrupada .= "<td style=\"text-align:center\" ><b>".$agCabecalho."</b></td>";
                     }
                 }

                 $xx = 0;
                 foreach($dadosAgrupados as $dA){
                     ($xx % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC";
                     $listaAgrupada .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";

                     foreach($dA as $k => $dd){
                         $kk[] = $k;
                     }
                     $ii = 0;
                     while($ii < count($dA)){

                         if($sqlAgrupador['link']){
                             if($sqlAgrupador['campo']){
                                 if(is_array($sqlAgrupador['campo'])){
                                     unset($arrCampos);
                                     foreach($sqlAgrupador['get'] as $cmp){
                                         $arrCampos[] = "{$cmp}={$dA[$cmp]}";
                                         $campos = implode("&",$arrCampos);
                                     }
                                 }else{
                                     $campos = "{$sqlAgrupador['get']}={{$dA[$kk[$sqlAgrupador['get']]]}}";
                                 }
                             }

                             $linkAg_a = "<a href=\"".$sqlAgrupador['link']."&".$campos."\" />";
                             $linkAg_b = " </a>";
                         }

                         if($kk[$ii] == $kk[0]){
                             $seta_filho = "<img src=\"../imagens/seta_filho.gif\" />";
                         }else{
                             $seta_filho = "";
                         }

                         if(!strstr($kk[$ii],"id") && !strstr($kk[$ii],"ordem") && !in_array($kk[$ii],$sqlAgrupador['arrOff'])){

                             if(in_array($kk[$ii],$sqlAgrupador['exibeLink'])){

                                 if(is_numeric($dA[$kk[$ii]])){
                                     $campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
                                     $listaAgrupada .= "<td>$seta_filho $linkAg_a $campo $linkAg_b</td>";
                                 }
                                 else{
                                     $listaAgrupada .= "<td>$seta_filho $linkAg_a {$dA[$kk[$ii]]} $linkAg_b</td>";
                                 }
                             }
                             else{
                                 if(is_numeric($dA[$kk[$ii]])){
                                     $campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
                                     $listaAgrupada .= "<td>$seta_filho $campo</td>";
                                 }
                                 else{
                                     $listaAgrupada .= "<td>$seta_filho {$dA[$kk[$ii]]}</td>";
                                 }
                             }

                         }
                        $ii++;

                     }
                     $listaAgrupada .= "</tr>";
                 $xx++;
                 }
             }
             $listaAgrupada .= "</table>";
         }

         $keys = array_keys($d);
         $j = 0;
        while($j < $num_colunas){
            if($sqlAgrupador && $keys[$j] == $keys[0] && $dadosAgrupados){
                $img = "<img onclick=\"exibeAgrupador('{$id}_{$id_span}')\" style=\"cursor:pointer;vertical-align: baseline;\" id=\"img_mais_{$id}_{$id_span}\" src=\"../imagens/mais.gif\" title=\"Abrir\" />
                        <img onclick=\"escondeAgrupador('{$id}_{$id_span}')\" style=\"cursor:pointer;display:none;vertical-align: baseline\" id=\"img_menos_{$id}_{$id_span}\" src=\"../imagens/menos.gif\" title=\"Fechar\" /> ";
                $span = "<tr style=\"display:none\" bgcolor='#EEE9E9' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#EEE9E9'\" id=\"tr_view_{$id}_{$id_span}\"><td colspan=\"$num_colunas\">$listaAgrupada</td></td></tr>";
                $id_span ++;
            }
            else{
                $img = "&nbsp;&nbsp;&nbsp;&nbsp;";
            }

            //Monta os links;
            if($link && $dadosAgrupados){
                $link_a = "<a href=\"{$link['link']}&{$link['get']}=".$d[$link['get']]."\" >";
                $link_b = "</a>";
            }else{
                $link_a = "";
                $link_b = "";
            }


            if(!strstr($keys[$j],"id") && !strstr($keys[$j],"ordem") && !in_array($keys[$j],$arrOff)){
                $tabela .= "<td><center>";
                if($link['campo'] == $keys[$j]){
                    $tabela .= $img.$link_a;
                }else{
                    $tabela .= $img;
                }
                 if(is_numeric($d[$keys[$j]])){
                     $campo = str_replace(",",".",number_format($d[$keys[$j]]));
                     $tabela .= $campo.$link_b."</center></td>";
                 }else{
                     if($link['campo'] == $keys[$j]){
                        $tabela .= $d[$keys[$j]].$link_b."</center></td>";
                    }else{
                        $tabela .= $d[$keys[$j]]."</center></td>";
                    }

                 }

            }

             if(!strstr($keys[$j],"ordem") && is_numeric($d[$keys[$j]])  && !in_array($keys[$j],$arrOff)){
                 $soma[$keys[$j]] += $d[$keys[$j]];
                 $campo_soma[] = $keys[$j];
             }
             $j++;

        }

         $tabela .= "</tr>";
         $tabela .= $span;

         $i++;
     }

     foreach($keys as $k => $k1){
          if(strstr($k1,"id")){
              unset ($keys[$k]);
          }
     }

     //Exibe Soma
     if($exibeSoma == "S"){
         $tabela .= "<tr bgcolor='DCDCDC' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='DCDCDC'\" >";
         $campo_soma = array_unique($campo_soma);
         foreach($keys as $k1 => $k){

             if(!in_array($k,$arrOff)){

                 if(in_array($k,$campo_soma)){
                     $tabela .= "<td><b>Total:</b> ".str_replace(",",".",number_format($soma[$k]))."</td>";
                 }else{
                     $tabela .= "<td></td>";
                 }
             }
         }
         $tabela .= "</tr>";
     }

     $tabela .= "</table>";
     $tabela .="<script>
     function exibeAgrupador(id){
         var img_mais = document.getElementById('img_mais_' +id);
         var img_menos = document.getElementById('img_menos_' +id);
         var tr_view = document.getElementById('tr_view_' +id);

         img_mais.style.display = 'none';
         img_menos.style.display = '';
         tr_view.style.display = '';

     }

     function escondeAgrupador(id){
         var img_mais = document.getElementById('img_mais_' +id);
         var img_menos = document.getElementById('img_menos_' +id);
         var tr_view = document.getElementById('tr_view_' +id);

         img_mais.style.display = '';
         img_menos.style.display = 'none';
         tr_view.style.display = 'none';

     }

                 </script>";

     echo $tabela;
}

function evtPegarDocCompra($coaid=null){
	global $db;

	$coaid = $coaid ? $coaid : $_SESSION['coaid'];

	$sql = "SELECT
				docid
			FROM
				evento.coadesao
			WHERE
				coaid = {$coaid}";

	return $db->pegaUm($sql);
}

function evtCriarDocCompra($coaid=null){
	global $db;

	$coaid = $coaid ? $coaid : $_SESSION['coaid'];

	if (!$coaid)
		return false;

	$docid = evtPegarDocCompra($coaid);

	if (!$docid){
		/*
		 * Pega tipo do documento "WORKFLOW"
		 */
		$sql = "SELECT
					tpdid
				FROM
					workflow.tipodocumento
				WHERE
					sisid =".EVT_SISID."
				AND
					tpdid = ". WF_TPDID_COMPRAS;

		$tpdid = $db->pegaUm($sql);
		/*
		 * Pega nome do evento
		 */
		$sql = "SELECT
					c.copnumprocesso
				FROM
					evento.coadesao as ca
					inner join evento.coprocesso as c on c.copid = ca.copid
				WHERE
					ca.coaid = $coaid";

		$tit = $db->pegaUm($sql);

		$docdsc = "Cadastramento Compras - " . $tit;
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
		/*
		 * Atualiza o $docid no coadesao
		 */
		$sql = "UPDATE evento.coadesao SET
					docid = '".$docid."'
				WHERE
					coaid = {$coaid}";

		$db->executar( $sql );
		$db->commit();
	}

	return $docid;
}

function montaAbasCompras( $linkAtual ){
	global $db;
	if( $linkAtual == '' ){
		return false;
	}
	$perfis = arrayPerfil();
	$res = array(
 					0 => array ( "descricao" => "Processos",
						    "id" 		=> "4",
						    "link" 		=> "/evento/evento.php?modulo=inicio&acao=C&submod=compra"
				  		  ),
				 	1 => array ( "descricao" => "Dados do Processo",
										    "id" 		=> "4",
										    "link" 		=> "/evento/evento.php?modulo=principal/cadProcesso&acao=A"
								  		  )  );
					 	if( $_SESSION['copid'] != '' ) {
							array_push($res,
											array ("descricao" => "Documentos Anexos",
													    "id"        => "3",
													    "link" 		=> "/evento/evento.php?modulo=principal/cadCompraAnexo&acao=A"
													   )  );
						}
						array_push( $res,
							 		array ("descricao" => "Registrar Demandas",
														    "id"		=> "2",
														    "link"		=> "/evento/evento.php?modulo=principal/cadCompraInfra&acao=A"
												  		   )  );
				 		array_push( $res,
					 		 		array ("descricao" => "Endereços de Entrega",
														    "id"		=> "1",
														    "link"		=> "/evento/evento.php?modulo=principal/cadCompraEnd&acao=A"
												  		   )  );
						array_push( $res,
							 		array ("descricao" => "Cadastrar Itens",
										    "id"		=> "1",
										    "link"		=> "/evento/evento.php?modulo=principal/cadCompraItem&acao=A"
						  		   						   )  );

	echo montarAbasArray($res, $_REQUEST['org'] ? false : $linkAtual);
}

//function pegarEntidInstituicao($usucpf){
//
//	global $db;
//	$sql = "select
//			pflcod
//	from
//		evento.usuarioresponsabilidade
//	where
//		usucpf = '$usucpf'";
//
//	$pflcod = $db->pegaUm($sql);
//
//	$sql2 = "select
//			distinct
//				ent.entid
//			from
//				evento.usuarioresponsabilidade ur
//			inner join
//				public.unidade p ON ur.unicod = p.unicod
//			inner join
//				entidade.entidade ent ON ur.unicod = ent.entunicod
//			inner join
//				entidade.entidadeendereco entEnd ON ent.entid = entEnd.entid
//			inner join
//				entidade.endereco ende ON ende.endid = entEnd.endid
//			inner join
//				evento.coenderecoentrega coend ON coend.entid = ent.entid
//			inner join
//				territorios.municipio mun on coend.muncod = mun.muncod
//			inner join
//				territorios.estado est on est.estuf = mun.estuf
//			where
//				ur.rpustatus = 'A' and
//				ur.usucpf = '$usucpf' and
//				ur.pflcod = $pflcod and
//				ur.prsano = '".$_SESSION['exercicio']."' and
//				coend.coendstatus = 'A'";
//
//	return $db->pegaUm( $sql2 );
//}

function verificaAdesao(){
	global $db;

	//if( !pegaCoaid($_SESSION['copid'],$_SESSION['unidade'])){
		$sql ="
		insert into evento.coadesao ( usucpf, docid, copid, usgid, coadatainclusao )
		values ( '".$_SESSION['usucpf']."', NULL, {$_SESSION['copid']}, {$_SESSION['unidade']}, 'now()' )
		returning coaid
		";
//ver($sql,d);
		$coaid = $db->pegaUm( $sql );
		$_SESSION['coaid'] = $coaid;
		evtCriarDocCompra();
		$db->commit();
		return $coaid;

	//}
}

function gravaGestor($coaid, $usgid, $usucpfgestor){
	global $db;
	$sql = "update evento.coadesao
			set usucpfgestor = '{$usucpfgestor}'
	 		where coaid = {$coaid} and usgid = {$usgid}";

	 $db->executar( $sql );

	return true;
}

function pegaGestor($coaid, $usgid){
	global $db;
	$sql = "select usucpfgestor
		    from evento.coadesao
			where coaid = {$coaid} and usgid = {$usgid}";

	 $usucpfgestor = $db->pegaUm( $sql );

	if( $usucpfgestor ){
		return $usucpfgestor;
	}else{
		return false;
	}


}


function pegaCoaid($copid, $usgid, $criaAdesao = true){
	global $db;
	$sql = "select coaid from evento.coadesao where copid = {$copid} and usgid = {$usgid}";
	$coaid = $db->pegaUm( $sql );


	if( $coaid ){
		$_SESSION['coaid'] = $coaid;
		return $coaid;
	}else{
		if($criaAdesao){
			return verificaAdesao();
		}
	}
	return false;
}

function validaTramiteCompras(){
	global $db;

	if($_SESSION['copid'] && $_SESSION['unidade']) {
		$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade']);

		$sql = " SELECT DISTINCT
							'<center><a href=\"javascript:carregaDetalheItemCadastrado('|| cd.cotid ||', ''cadastrado'', \''|| c.coidsc ||'\');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\"> <a href=\"#\" onclick=\"javascript:excluirItemProcesso('|| cd.cotid ||', ''cadastrado'');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>'  as acao,
	                     	c.coidsc,
	            			c.coiqtde,
	            			c.coivlrreferenciamin,
	            			c.coivlrreferenciamax
					FROM evento.codemandaitem AS cd
					INNER JOIN evento.coitemprocesso AS ci ON ci.cotid = cd.cotid
					INNER JOIN evento.coitem AS c ON c.coiid = ci.coiid
					WHERE cd.coaid = {$coaid}";
		$rsItens = $db->carregar( $sql );

		$sql = "select
					count(ee.coeid) as c
				FROM
					evento.coenderecoentrega  ee
				INNER JOIN evento.coadesao a on a.coaid = ee.coaid
				WHERE
					a.copid = ". $_SESSION['copid'] ."
				AND a.usgid = ". $_SESSION['unidade'];

		$rsEndereco = $db->pegaUm( $sql );
	}


	if($rsItens && $rsEndereco) {
		return true;
	} else {
		return false;
	}
}

function verificaUnidadesPermitidadas(){
	global $db;

	# Array de perfis que veem todas as unidades
	$arPerfisVerTodas = array(EVENTO_PERFIL_CGCC,
						 	  EVENTO_PERFIL_CONSULTA,
						 	  EVENTO_PERFIL_PERFIL_EMPRESA,
						 	  EVENTO_PERFIL_SUPER_USUARIO
							  );
	# Array de perfis que so veem somente as unidades atribuidas
	$arPerfisUnidadesAtribuidas = array(EVENTO_PERFIL_ORDENADOR_DESPESA_COMPRAS,
										EVENTO_PERFIL_CONSULTA_COMPRAS,
							 	  		EVENTO_PERFIL_DEMANDANTE_COMPRAS);

	# Array de perfis vinculado ao perfil do usuário
	$arPerfilVinculado = array();
	# Array de Unidade Visiveis para o perfil do usuário
	$arUnidadesVisiveis = array();
	$arUnidadesVisiveisTemp = array();

	# Recuperamos todos o perfis cadastrado para o usuário logado
	$arPerfis = arrayPerfil();
	foreach($arPerfis as $perfil){
		if(in_array($perfil,$arPerfisVerTodas)){
			return true;
		} elseif(in_array($perfil,$arPerfisUnidadesAtribuidas)){
			$arPerfilVinculado[] = $perfil;
		}
	}

	if(is_array($arPerfilVinculado)){
		$sql = "SELECT usgid FROM evento.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod in (". implode(",", $arPerfilVinculado).")";
		$arUnidadesVisiveisTemp = $db->carregar($sql);

		if(is_array($arUnidadesVisiveisTemp)){
			extract($arUnidadesVisiveisTemp);
			foreach($arUnidadesVisiveisTemp as $unidadesVisiveis){
				$arUnidadesVisiveis[] = $unidadesVisiveis['usgid'];
			}
		} else {
			return "Não existe Unidades atribuidas ao perfil para este CPF: {$_SESSION['usucpf']}.";
		}
	}

	return $arUnidadesVisiveis;
}

function removerdeclaracao($dados = false) {
	global $db;

	if(!$dados)
		$dados['decid'] = $db->pegaUm("SELECT decid FROM evento.declaracao WHERE copid='".$_SESSION['copid']."' AND usgid='".$_SESSION['unidade']."' AND decstatus='A'");

	$sql = "SELECT arqid FROM evento.declaracao WHERE decid = '".$dados['decid']."'";
	$arqid = $db->pegaUm($sql);

	$sql = "DELETE FROM evento.declaracao WHERE decid='".$dados['decid']."'";
	$db->executar($sql);
	//deletando pdf em public.arquivo
	if($arqid){
		$sql ="DELETE FROM public.arquivo WHERE arqid = '$arqid'";
		$db->executar($sql);
	}
	$db->commit();
	//deletando o arquivo pdf físico do servidor
	if($arqid){
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;

		if(file_exists($caminho)){
			unlink($caminho);
		}
	}
	echo "<script>
			alert('Declaração removida com sucesso.');
			window.location = '?modulo=principal/formDeclaracao&acao=A';
		  </script>";
}

function verificaDeclaracao(){
	global $db;
	$sql = "SELECT
				decid
			FROM
				evento.declaracao dec
			INNER JOIN evento.coprocesso cop on dec.copid = cop.copid
			WHERE
				cop.copdatalimite >= CURRENT_DATE
			AND dec.usgid = '".$_SESSION['unidade']."'
			AND dec.copid = '".$_SESSION['copid']."' AND decstatus='A'";

	$decid = $db->pegaUm($sql);
	if(!$decid){
		return false;
	} else {
		return true;
	}
}

function pegaPerfilArray($cpf,$sisid){
	global $db;
	$sql = "select p.pflcod from seguranca.perfilusuario pu inner join seguranca.perfil p on pu.pflcod = p.pflcod where pu.usucpf = '$cpf' and p.pflstatus = 'A' and p.sisid = $sisid;";
	return $db->carregarColuna($sql);
}

function possuiPerfil( $pflcods ){

	global $db;

	if ($db->testa_superuser()) {
		return true;
	}else{
		if ( is_array( $pflcods ) ){
			$pflcods = array_map( "intval", $pflcods );
			$pflcods = array_unique( $pflcods );
		} else {
			$pflcods = array( (integer) $pflcods );
		} if ( count( $pflcods ) == 0 ) {
			return false;
		}
		$sql = "select
					count(*)
			from seguranca.perfilusuario
			where
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode( ",", $pflcods ) . " ) ";
		return ($db->pegaUm( $sql ) > 0);
	}
}

function verificaEstadoDocumento( $docid ){
	global $db;
	$sql = "SELECT esdid FROM workflow.documento
			WHERE docid = {$docid}";
	$estado = $db->pegaUm( $sql );
	if( $estado )
		return $estado;
}

function permissaoAlterar($coaid){
	global $db;

	if($coaid){
		$docid = evtPegarDocCompra($coaid);
		if($docid){
			$estado = verificaEstadoDocumento( $docid );
		}
	}

	if ($db->testa_superuser() || possuiPerfil(EVENTO_PERFIL_ORDENADOR_DESPESA_COMPRAS) || possuiPerfil(EVENTO_PERFIL_DEMANDANTE_COMPRAS) ) {
		if($estado == EM_ANALISE_SAA_WF){
			return false;
		}
		return true;
	} else {
		if( $estado == AGUARDANDO_APROVACAO_CORD_WF ){
			return false;
		}
	}

	return false;
}

function verificaSessao($boVerificaCopid = false){

	if(!$_SESSION['unidade']){
		echo "<script>
				alert('Favor selecionar uma Unidade');
				window.location.href = 'evento.php?modulo=principal/inicioCompraUnidade&acao=A';
			  </script>";
		die;
	}
	if($boVerificaCopid){
		if(!$_SESSION['copid'] && $_SESSION['unidade']){
			echo "<script>
					alert('Favor selecionar um Processo.');
					window.location.href = 'evento.php?modulo=principal/listaProcessoUnidade&acao=A';
				  </script>";
			die;
		}
	}
	return false;
}
    function temPerfilEmpresa(){
        global $db;
        $perfis = arrayPerfil();
        if(in_array(PERFIL_EMPRESA, $perfis)){
            return true;
        }
        return false;
    }

function verificaSessaoPagina(){

	if(!$_SESSION['ctrid']){
			echo "<script>
					alert('Sessão expirou. Favor selecionar o contrato novamente.');
					window.location.href = 'evento.php?modulo=principal/inicioContrato&acao=A';
				  </script>";
			die;
		}
}

function enviarEmailPorEstadoWorkflow(){

	global $db, $docid;

	$sql = "select
				evetitulo,
				to_char(evedatainicio::date,'DD/MM/YYYY') as evedatainicio,
				to_char(evedatafim::date,'DD/MM/YYYY') as evedatafim
			from evento.evento
			where docid = {$_REQUEST['docid']}";

	$rs = $db->pegaLinha($sql);

	// Demandate
	$sql = "select usuemail from seguranca.usuario where usucpf = '{$_SESSION['usucpf']}'";
	$emailDemandate = $db->pegaUm($sql);

	$arrEmails 				= array();

	// Segue os arrays de emails
	$arTodos 				= array($_SESSION['email_sistema']);

	$arEmpresa 				= array("maira@fjproducoes.com.br",
									"patrícia@fjproducoes.com.br");

	// Todos
	$esdidTodos 			= array(
								EM_ANALISE_COMITE_WF,
								APROVADO_PELO_COMITE_WF,
								PROJETO_FINALIZADO_WF
								);

	// Empresa
	$esdidEmpresa 			= array(
								APROVADO_PELO_COMITE_WF,
								ADEQUACAO_PROJETO_WF,
								PROJETO_FINALIZADO_WF,
								EMISSAO_EMPENHO_WF,
								PAGAMENTO_NF_WF
								);
	// Orçamento SPO
	$esdidSPO    			= array(
								ELABORACAO_CDO_WF
								);

	// SPO (Subsecretário)
	$esdidSPOSubsecretario 	= array(
								EMISSAO_CDO_WF
								);

	// Área Demandante
	$esdidAreaDemandante 	= array(
								EMISSAO_CDO_WF,
								EMISSAO_EMPENHO_WF,
								ATESTO_NF_WF
								);

	// SAA
	$esdidSAA			 	= array(
								INSTRUCAO_PROCESSO_WF,
								EMISSAO_EMPENHO_WF,
								ATESTO_NF_WF,
								PAGAMENTO_NF_WF
								);

	// Comitê de Eventos
	$esdidComiteEventos	 	= array(
								ATESTO_NF_WF,
								PAGAMENTO_NF_WF
								);

	// Adiciona Todos
	if(in_array($_REQUEST['esdid'], $esdidTodos))
		array_push($arrEmails, $arTodos);

	// Adiciona Empresas
	if(in_array($_REQUEST['esdid'], $esdidEmpresa))
		array_push($arrEmails, $arEmpresa);

	// Adiciona SPO
	if(in_array($_REQUEST['esdid'], $esdidSPO))
		array_push($arrEmails, "esdidSPO@temp.com.br");

	// Adiciona SPO subsecretário
	if(in_array($_REQUEST['esdid'], $esdidSPOSubsecretario))
		array_push($arrEmails, "esdidSPOSubsecretario@temp.com.br");

	// Adiciona Área Demandate
	if(in_array($_REQUEST['esdid'], $esdidAreaDemandante))
		array_push($arrEmails, "esdidAreaDemandante@temp.com.br");

	// Adiciona SAA
	if(in_array($_REQUEST['esdid'], $esdidSAA))
		array_push($arrEmails, "esdidSAA@temp.com.br");

	// Adiciona Comite de Eventos
	if(in_array($_REQUEST['esdid'], $esdidComiteEventos))
		array_push($arrEmails, "esdidComiteEventos@temp.com.br");

	$remetente = array('nome'=>REMETENTE_WORKFLOW_NOME, 'email'=>REMETENTE_WORKFLOW_EMAIL);

	$assunto   = "[SIMEC] Módulo de Eventos";

	// retirar quando validar essa funcao
	if($_REQUEST['esdid'] == EM_ANALISE_COMITE_WF){

		if(!verificaVoltaEstadoWorflow()){

			if(verificaPrazoConformeComite()){
				return true;
			} else {
				return false;
			}
		}

	} else {

		return true;
	}

	if($_REQUEST['esdid'] == EM_ANALISE_COMITE_WF){
		$mailBody = '
		<b>Prezados Senhores,</b><br><br>
		Informamos que o evento nº '.$_SESSION['evento']['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi cadastrado no SIMEC e enviado para análise e aprovação do comitê de eventos.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o SIMEC">Clique Aqui para acessar o SIMEC.</a><br><br>
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(!verificaVoltaEstadoWorflow()){

			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}

	} else if($_REQUEST['esdid'] == APROVADO_PELO_COMITE_WF){

		$mailBody = '
		<b>Prezados Senhores,</b><br><br>
		Informamos que o evento nº '.$_SESSION['evento']['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi cadastrado no SIMEC foi aprovado pelo comitê de eventos.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o SIMEC">Clique Aqui para acessar o SIMEC.</a><br><br>
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){

			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}

	} else if($_REQUEST['esdid'] == ADEQUACAO_PROJETO_WF){

		$mailBody = '
		<b>Prezados Senhores,</b><br><br>
		Informamos que o evento nº '.$_SESSION['evento']['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi lançado de forma preliminar.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o SIMEC">Clique Aqui para acessar o SIMEC.</a><br><br>
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){

			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}

	} else if($_REQUEST['esdid'] == PROJETO_FINALIZADO_WF){

		$mailBody = '
		<b>Prezados Senhores,</b><br><br>
		Informamos que o evento nº '.$_SESSION['evento']['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi  lançado de forma definitiva.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o SIMEC">Clique Aqui para acessar o SIMEC.</a><br><br>
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){

			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}

	} else if($_REQUEST['esdid'] == ELABORACAO_CDO_WF){

		$mailBody = '
		<b>Prezados Senhores,</b><br><br>
		Informamos que o evento nº '.$_SESSION['evento']['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi cadastrado no SIMEC sendo necessária a preparação da  emissão da CDO.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o SIMEC">Clique Aqui para acessar o SIMEC.</a><br><br>
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){

			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}

	} else if($_REQUEST['esdid'] == EMISSAO_CDO_WF){

		$mailBody = '
		<b>Prezados Senhores,</b><br><br>
		Informamos que o evento nº '.$_SESSION['evento']['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		está apto para emissão da CDO.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o SIMEC">Clique Aqui para acessar o SIMEC.</a><br><br>
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){

			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}

	} else if($_REQUEST['esdid'] == INSTRUCAO_PROCESSO_WF){

		$mailBody = '
		<b>Prezados Senhores,</b><br><br>
		Informamos que o evento nº '.$_SESSION['evento']['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		está apto para impressão dos documentos relativos ao evento<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o SIMEC">Clique Aqui para acessar o SIMEC.</a><br><br>
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){

			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}

	} else if($_REQUEST['esdid'] == EMISSAO_EMPENHO_WF){

		$mailBody = '
		<b>Prezados Senhores,</b><br><br>
		Informamos que o evento nº '.$_SESSION['evento']['eveid'].' - "'.$rs['evetitulo'].'" a ser realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		está apto para emissão da Nota de Empenho e Ordem de Serviço.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o SIMEC">Clique Aqui para acessar o SIMEC.</a><br><br>
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){

			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}

	} else if($_REQUEST['esdid'] == ATESTO_NF_WF){

		$mailBody = '
		<b>Prezados Senhores,</b><br><br>
		Informamos que a Nota Fiscal relativa ao evento nº '.$_SESSION['evento']['eveid'].' - "'.$rs['evetitulo'].'" realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi realizado e a correspondente NF foi emitida para pagamento.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o SIMEC">Clique Aqui para acessar o SIMEC.</a><br><br>
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){

			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}

	} else if($_REQUEST['esdid'] == PAGAMENTO_NF_WF){

		$mailBody = '
		<b>Prezados Senhores,</b><br><br>
		Informamos que o pagamento da Nota fiscal relativa ao evento nº '.$_SESSION['evento']['eveid'].' - "'.$rs['evetitulo'].'" realizado no período de '.$rs['evedatainicio'].' à '.$rs['evedatafim'].',<br>
		foi realizado.<br><br>
		<a href="http://simec.mec.gov.br/" title="Acessar o SIMEC">Clique Aqui para acessar o SIMEC.</a><br><br>
		Atenciosamente,<br><br>
		SIMEC - Módulo de Eventos<br>
		';

		if(verificaVoltaEstadoWorflow()){

			enviar_email($remetente, $_SESSION['email_sistema'], $assunto, $mailBody, $arrEmails );
		}

	}

	return true;
}

    function verificaPrazoConformeComite(){
	global $db;

	$sql = "
            SELECT  ev.evetitulo,
                    ev.ungcod,
                    ev.tpeid,
                    ev.evedatainicio,
                    ev.evedatafim,
                    ev.eveemail,
                    ev.evenumeropi,
                    ev.evenumeroprocesso,
                    ev.evecustoprevisto,
                    ev.evepublicoestimado,
                    ev.evequantidadedias,
                    ev.muncod,
                    ev.estuf,
                    ev.sevid,
                    ev.eveqtdpassagemaerea,
                    u.ungdsc,
                    us.usunome,
                    ev.docid,
                    ev.eveurgente,
                    ev.rcoid,
                    ev.endid,
                    to_char(ev.evedatainclusao::date,'DD/MM/YYYY') AS evedatainclusao,
                    ev.eveanopi

            FROM evento.evento AS ev

            LEFT JOIN evento.tipoevento AS te ON te.tpeid = ev.tpeid
            LEFT JOIN public.unidadegestora AS u ON ev.ungcod = u.ungcod
            LEFT JOIN seguranca.usuario AS us ON us.usucpf = ev.usucpf

            WHERE ev.eveid = {$_SESSION['evento']['eveid']};
        ";
	$rsDadosEvento = $db->carregar( $sql );

	if($_REQUEST['esdid'] == EM_ANALISE_COMITE_WF){

            $data = new Data();
            $retorno = $data->timeStampDeUmaData( date("d/m/Y") );
            $retorno1 = $data->timeStampDeUmaData($rsDadosEvento[0]['evedatainicio']);
            $segundos_diferenca = $retorno - $retorno1;
            $dias_diferenca = $segundos_diferenca / (60 * 60 * 24);
            $dias_diferenca = abs($dias_diferenca);
            $dias_diferenca = floor($dias_diferenca);

            if( $rsDadosEvento[0]['evepublicoestimado'] <= 50 ){
                $diferenca_permitida = 30;

            }elseif( ( $rsDadosEvento[0]['evepublicoestimado'] > 50 ) && ( $rsDadosEvento[0]['evepublicoestimado'] <= 250 ) ){
                $diferenca_permitida = 45;

            }elseif( ( $rsDadosEvento[0]['evepublicoestimado'] > 250 ) && ( $rsDadosEvento[0]['evepublicoestimado'] <= 500 ) ){
                $diferenca_permitida = 60;
            }
            elseif( $rsDadosEvento[0]['evepublicoestimado'] > 500 ){
                $diferenca_permitida = 90;
            }

            if(  $dias_diferenca >= $diferenca_permitida ){
                $evedataurgente = "f";
                $eveurgente     = "f";
            }else{
                $evedataurgente = "t";
                $eveurgente     = "t";
            }

            if( $rsDadosEvento[0]['adreverendo'] ){
                $adreferendum = "t";
                $eveurgente   = "t";
            }else{
                $adreferendum = "f";
                $eveurgente   = "f";
            }

            #verificando se valerá a regra de AD-REFERENDUM para o perfil.
            $perfis = arrayPerfil();
            $boAdreferendum = true;
            if( !in_array( PERFIL_SUPER_USUARIO, $perfis) && !in_array(PERFIL_SAA, $perfis)){
                if( $adreferendum != $evedataurgente){
                    $boAdreferendum = false;
                }
            }

            $arEvents =  explode( "_", verificaEventos( $rsDadosEvento[0]['ungcod'] ) );
            $numEventosSemNota = $arEvents[0];
            $numEventosSemAval = $arEvents[1];

            if( $numEventosSemNota < MAX_EVENTOS_SEM_NOTA || $numEventosSemAval < MAX_EVENTOS_SEM_NOTA ){
                if( !$boAdreferendum ){
                    alert("A data de início do evento está fora do prazo, de acordo com as regras do comitê. Entre em contato com a SAA.");
                    echo "<script>window.close();</script>";
                    return false;
                } else {
                    return true;
                }
            } else {
                echo '
                    <script type="text/javascript">
                        alert( "Relatórios Técnicos em Aberto, ou Avaliação de eventos não preenchida." );
                        window.location.href = "?modulo=inicio&acao=C"
                    </script>
                ';
                return false;
            }
	} else {
            return true;
	}
    }

    function eventoEnviaAnaliseComite(){
        global $db;

        if(!$_SESSION['evento']['eveid']){
            return "Sessão expirou. Favor entrar novamente no sistema de evetos.";
        }

        #VERIFICA ANEXO.
        $sql = "SELECT count(axpid) FROM evento.anexoevento where axestatus='A' and eveid = ".$_SESSION['evento']['eveid'];
        $verificaAnexo = $db->pegaUm($sql);
        if($verificaAnexo == 0){
            return "É necessário anexar um arquivo.";
        }

        $evedatainclusao = buscaDataInclusaoEvento();
        $data_nova_regra_evento = strtotime(DATA_NOVO_REGRA_EVENTO_NOVA_INFRA);

        if( $evedatainclusao < $data_nova_regra_evento ){
            #VERIFICA ITENS DE INFRA ESTRUTURA.
            $sql = "SELECT count(ievid) FROM evento.itemevento where ievstatus='A' and eveid = ".$_SESSION['evento']['eveid'];
            $verificaItemInfra = $db->pegaUm($sql);
            if($verificaItemInfra == 0){
                return "É necessário cadastrar pelo menos um item na aba Infraestrutura.";
            }
        }else{
            #VERIFICA ITENS DE INFRA ESTRUTURA.
            $sql = "SELECT count(itcid) AS itcid FROM evento.itemconsumo where eveid = {$_SESSION['evento']['eveid']};";
            $itcid = $db->pegaUm($sql);
            if($itcid == 0){
                return "É necessário cadastrar pelo menos um item na aba Infraestrutura.";
            }
        }
        return 'OK';
    }

function verificaDiasEnviarAnalise()
{
	global $db;

	$sql = "SELECT evedatainicio, evepublicoestimado, evenumeropi, eveanopi FROM evento.evento where eveid = ".$_SESSION['evento']['eveid'];
	$rsDadosEvento = $db->pegaLinha($sql);

	if($rsDadosEvento){

		$data = new Data();
		$retorno = $data->timeStampDeUmaData( date("d/m/Y") );
		$retorno1 = $data->timeStampDeUmaData($rsDadosEvento['evedatainicio']);
		$segundos_diferenca = $retorno - $retorno1;
		$dias_diferenca = $segundos_diferenca / (60 * 60 * 24);
		$dias_diferenca = abs($dias_diferenca);
		$dias_diferenca = floor($dias_diferenca);

		if( $rsDadosEvento['evepublicoestimado'] <= 50 ){
			$diferenca_permitida = 30;
		}
		elseif( ( $rsDadosEvento['evepublicoestimado'] > 50 ) AND ( $rsDadosEvento['evepublicoestimado'] <= 250 ) ){
			$diferenca_permitida = 45;
		}
		elseif( ( $rsDadosEvento['evepublicoestimado'] > 250 ) AND ( $rsDadosEvento['evepublicoestimado'] <= 500 ) ){
			$diferenca_permitida = 60;
		}
		elseif( $rsDadosEvento['evepublicoestimado'] > 500 )	{
			$diferenca_permitida = 90;
		}

		if(  $diferenca_permitida >= $dias_diferenca ){
                    return false; // "A data de início do evento está fora do prazo, de acordo com as regras do comitê. A data de início deverá acontecer após $diferenca_permitida dias. Entre em contato com a SAA.";
		}
	}
	return true;
}

function dPagamentoPermissaoEdicao(){
	global $db;

	if($_SESSION['evento']['eveid']){
		$sql = "
			Select dpaid From evento.documentopagamento where eveid = '".$_SESSION['evento']['eveid']."'
		";
		$permite = $db->pegaLinha($sql);
		return $permite['dpaid'];
	}
}

function dPagamentoWorkFlow(){
	global $db;

	if($_SESSION['evento']['eveid']){
		$sql = "
		Select dpaid From evento.documentopagamento where eveid = '".$_SESSION['evento']['eveid']."'
		";
		$permite = $db->pegaLinha($sql);
	}

	if( $permite['dpaid'] != ''){
		return true;
	}else{
		return false;
	}
}

    function eventoPermissaoEdicao(){
	global $db;

	if($_SESSION['evento']['eveid']){

		$sql = "
                    SELECT  d.esdid
                    FROM evento.evento e
                    INNER JOIN workflow.documento as d on d.docid = e.docid
                    WHERE e.eveid = {$_SESSION['evento']['eveid']};
                ";
		$esdid = $db->pegaUm($sql);

		if($esdid != EM_CADASTRAMENTO_WF){
                    return '
                        <script>
                            var obj = document.getElementsByTagName("input");
                            var total = document.getElementsByTagName("input").length;

                            for(i=0; i<total; i++){
                                if( obj[i].type == "hidden" ){
                                    obj[i].disabled = false;
                                }else{
                                    obj[i].disabled = true;
                                }
                            }
                            obj = document.getElementsByTagName("select");
                            total = document.getElementsByTagName("select").length;

                            for(i=0; i<total; i++){
                                obj[i].disabled = true;
                            }
                        </script>
                    ';
		} else {
                    return '';
		}
	} else {
            return '';
	}
    }

function verificaVoltaEstadoWorflow(){

	global $docid, $db;

	$sql = "select * from workflow.historicodocumento where docid = {$docid} and aedid = {$_REQUEST['aedid']} order by hstid desc";
	$boVoltou = $db->pegaLinha($sql);

	if($boVoltou){
		return true;
	} else {
		return false;
	}

}

function verificarAnexoNF(){

	global $db;

	$sql = "select axpid from evento.anexoevento aev
			inner join public.arquivo arq on aev.arqid = arq.arqid
			where arq.arqstatus = 'A'
			and  aev.eveid = '{$_SESSION['evento']['eveid']}'";

	$anexo = $db->carregar($sql);

	if($anexo){

		return true;

	} else {

		return false;
	}

}

function mostraAbaDocPagamento($eveid){
	global $db;

	$docid = evtCriarDoc($_SESSION['evento']['eveid']);
	$esdid = verificaEstadoDocumento($docid);

	if($esdid == AGUARDANDO_PAGAMENTO_EVENTO_WF || $esdid == PROJETO_FINALIZADO_WF){
		return true;
	}else{
		return false;
	}
	/*
	if($eveid){
		$sql = "select to_char(evedatafim, 'DD/MM/YYYY') as evedatafim  from evento.evento where eveid = $eveid";
		$evedatafim = $db->pegaUm($sql);

		$dataAtual = date('d/m/Y');
		$obData = new Data();

		return true; //retirar esta linha antes de entrar pra produção.
		$retorno = $obData->diferencaEntreDatas(  $dataAtual, $evedatafim, 'maiorDataBolean', null, 'dd/mm/yyyy');

		if($retorno && possuiPerfil(EVENTO_PERFIL_SAA_FINANCEIRO)){
			return true;
		}
	}

	return false;
	*/
}

function mostraAbaDocOS($eveid)
{
	global $db;

	if($eveid){
		$sql = "select oseid  from evento.ordemservico where eveid = $eveid";
		$oseid = $db->pegaUm($sql);

		if($oseid){
			return true;
		}
	}

	return false;
}

    function carregaDados( $id ){
	global $db;

	$sql = "
            SELECT  precodpregao,
                    predescpregao,
                    TO_CHAR( preiniciovig, 'dd/mm/YYYY') as preiniciovig,
                    TO_CHAR( prefimvig, 'dd/mm/YYYY') as prefimvig,
                    trim(to_char(prevalorcontratado,'999g999g999g999d99')) as prevalorcontratado,
                    trim(to_char(prevalorempenhado,'999g999g999g999d99')) as prevalorempenhado,
                    prenumprocesso,
                    precnpj,
                    prerazaosocial,
                    prenumcontrato
            FROM evento.pregaoevento_old epe
            WHERE epe.preid = {$id};
        ";
	$arrResp = $db->pegaLinha( $sql );

	echo $arrResp['precodpregao'] .'|'.$arrResp['predescpregao'] .'|'.$arrResp['preiniciovig'] .'|'.$arrResp['prefimvig'] .'|'.$arrResp['prevalorcontratado'] .'|'.$arrResp['prevalorempenhado'] .'|'.$arrResp['prenumprocesso'] .'|'.formatar_cpf_cnpj($arrResp['precnpj']). '|'.$arrResp['prerazaosocial']. '|' .$arrResp['prenumcontrato'] ;
    }
/*

function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}
*/

    #LEGADO - USADO NA UNIDADE RECURSO.
    function carregaDadosPregao( $id ){
        global $db;

        $sql = "
            SELECT  precodpregao,
                    prenumprocesso,
                    prevalorcontratado
            FROM  evento.pregaoevento_old epe

            WHERE epe.preid = {$id}
        ";
        return $arrResp = $db->pegaLinha( $sql );
    }

    #LEGADO - UNIDADE RECURSO
    function carregaDadosUnidade( $id ){
        global $db;

        $sql = "
            SELECT  '<center>
                        <img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('||ur.ureid||')\" \" border=0 alt=\"Ir\" title=\"Alterar\">  ' ||
                        '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('||ur.ureid||');\" border=0 alt=\"Ir\" title=\"Excluir\">
                    </center>' as acao,

                    '<a href=\"javascript:void(0);\" onclick=\"exibirExtrato(' || ur.ureid || ')\">' || ug.ungdsc || '</a>' as ungdsc,
                    ureordenador,
                    ureordenadorsub,
                    ur.urevalorrecurso AS limite,
                    coalesce(ur.urevalorsaldo,0) AS saldo
            FROM evento.unidaderecurso_old ur

            INNER JOIN public.unidadegestora ug ON ug.ungcod = ur.ungcod

            WHERE preid = {$id}
            order by ug.ungdsc
        ";
        return $sql;
    }

    #LEGADO - UNIDADE RECURSO
    function carregaDadosUnidadePorUreid( $id ){
        global $db;

        $sql = "
            SELECT  ug.ungcod,
                    ur.ureordenador,
                    ur.ureordenadorsub,
                    ur.urevalorrecurso AS limite,
                    ur.urevalorsaldo AS saldo
            FROM evento.unidaderecurso_old ur

            INNER JOIN public.unidadegestora ug ON ug.ungcod = ur.ungcod

            WHERE ureid = {$id}
        ";
        return $db->pegaLinha( $sql );
    }

function cabecalhoContrato( $id ){
	global $db;

	$sql = "SELECT
				ctr.ctrnum || '/' || ctr.ctrano as contrato,
				ent.entnome as contratada
  			FROM
				evento.ctcontrato ctr
			INNER JOIN
				entidade.entidade ent on ent.entid = ctr.entidcontratada
			WHERE
				ctr.ctrid = ".$id;

	$arrContrato = $db->pegaLinha( $sql );

	echo "
		<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
			<tbody>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Contrato:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$arrContrato['contrato']."</td>
				</tr>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Contratada:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$arrContrato['contratada']."</td>
				</tr>
			</tbody>
		</table>";

}

function retornaUngcods($perfils=Array()){

	global $db;

	$ungcods = Array();

	if( possuiPerfil( $perfils ) ){
		$sql = "SELECT DISTINCT
					uni.ungcod
				FROM
					evento.usuarioresponsabilidade ur
				INNER JOIN public.unidadegestora uni ON
					uni.ungcod = ur.ungcod AND
					uni.ungcod = '%s' AND
					uni.ungstatus = 'A'
				INNER JOIN seguranca.perfil pfl ON
					pfl.pflcod = ur.pflcod AND
					pfl.pflcod = '" . $pflcod . "'
				where
					ur.rpustatus = 'A' and
					ur.usucpf <> '" . $_SESSION['usucpf'] . "'";
		$ungcods = $db->pegaColuna($sql);
	}
	return $ungcods;
}

//Workflow Solicitação de Ajuda de Custo (Diárias)
function wf_condicao_solicitacao(){

//	global $db;
//
//	if($db->testa_superuser()){
//		return true;
//	}else{
//		$ungcods = retornaUngcods(Array(EVENTO_PERFIL_SOLICITADOR));
//
//		if(is_array($ungcods)){
//			$sql = "SELECT
//						ungcod
//					FROM
//						evento.solicitacaodiaria
//					WHERE
//						ungcod in ('".implode('\',\'',$ungcods)."')
//						AND solid = ".$_SESSION['evento']['solid'];
//			$sol = $db->carregar($sql);
//			if(is_array($sol)){
//				return true;
//			}else{
//				return false;
//			}
//		}else{
			return true;
//		}
//	}
}

function wf_condicao_validacao(){

	global $db;

//	if($db->testa_superuser()){
//		return true;
//	}else{
//		$ungcods = retornaUngcods(Array(EVENTO_PERFIL_VALIDADOR));
//
//		if(is_array($ungcods)){
//			$sql = "SELECT
//						ungcod
//					FROM
//						evento.solicitacaodiaria
//					WHERE
//						ungcod in ('".implode('\',\'',$ungcods)."')
//						AND solid = ".$_SESSION['evento']['solid'];
//			$sol = $db->carregar($sql);
//			if(is_array($sol)){
//				return true;
//			}else{
//				return false;
//			}
//		}else{
			return true;
//		}
//	}
}

function wf_condicao_retorno_validacao(){

	global $db;

//	if($db->testa_superuser()){
//		return true;
//	}else{
//		$ungcods = retornaUngcods(Array(EVENTO_PERFIL_VALIDADOR));
//
//		if(is_array($ungcods)){
//			$sql = "SELECT
//						ungcod
//					FROM
//						evento.solicitacaodiaria
//					WHERE
//						ungcod in ('".implode('\',\'',$ungcods)."')
//						AND solid = ".$_SESSION['evento']['solid'];
//			$sol = $db->carregar($sql);
//			if(is_array($sol)){
//				return true;
//			}else{
//				return false;
//			}
//		}else{
			return true;
//		}
//	}
}

function wf_condicao_autorizacao(){

	global $db;

//	if($db->testa_superuser()){
//		return true;
//	}else{
//		$ungcods = retornaUngcods(Array(EVENTO_PERFIL_AGENTE_FINANCEIRO));
//
//		if(is_array($ungcods)){
//			$sql = "SELECT
//						ungcod
//					FROM
//						evento.solicitacaodiaria
//					WHERE
//						ungcod in ('".implode('\',\'',$ungcods)."')
//						AND solid = ".$_SESSION['evento']['solid'];
//			$sol = $db->carregar($sql);
//			if(is_array($sol)){
//				return true;
//			}else{
//				return false;
//			}
//		}else{
			return true;
//		}
//	}
}

function wf_condicao_retorno_autorizacao(){

	global $db;

//	if($db->testa_superuser()){
//		return true;
//	}else{
//		$ungcods = retornaUngcods(Array(EVENTO_PERFIL_AGENTE_FINANCEIRO));
//
//		if(is_array($ungcods)){
//			$sql = "SELECT
//						ungcod
//					FROM
//						evento.solicitacaodiaria
//					WHERE
//						ungcod in ('".implode('\',\'',$ungcods)."')
//						AND solid = ".$_SESSION['evento']['solid'];
//			$sol = $db->carregar($sql);
//			if(is_array($sol)){
//				return true;
//			}else{
//				return false;
//			}
//		}else{
			return true;
//		}
//	}
}


function wf_condicao_pagamento(){

	global $db;

	$sql = "SELECT
				solordembancaria
			FROM
				evento.solicitacaodiaria
			WHERE
				solordembancaria not like 'NULL' AND
				solordembancaria is not null AND
				solid = ".$_SESSION['evento']['solid'];
	$sol = $db->carregar($sql);
	if(is_array($sol)){
		return true;
	}else{
		return "Solicitação sem ordem bancária informada.";
	}
//
//	if($db->testa_superuser()){
//		return true;
//	}else{
//		$ungcods = retornaUngcods(Array(EVENTO_PERFIL_AGENTE_FINANCEIRO));
//
//		if(is_array($ungcods)){
//			$sql = "SELECT
//						ungcod
//					FROM
//						evento.solicitacaodiaria
//					WHERE
//						ungcod in ('".implode('\',\'',$ungcods)."')
//						AND solid = ".$_SESSION['evento']['solid'];
//			$sol = $db->carregar($sql);
//			if(is_array($sol)){
//				return true;
//			}else{
//				return false;
//			}
//		}else{
			return true;
//		}
//	}
}

function excluirSolicitacao( $solid ){
	global $db;

	$sql = "SELECT
				solcomplemento
			FROM
				evento.solicitacaodiaria
			WHERE
				solid = ".$solid;
	$solidOriginal = $db->pegaUm($sql);

	if($solidOriginal){
		$sql = "UPDATE evento.solicitacaodiaria SET
					solstatus = 'A'
				WHERE
					solid = ".$solidOriginal;
		$db->executar($sql);
	}

	$sql = "UPDATE evento.solicitacaodiaria SET
				solstatus = 'I'
			WHERE
				solid = ".$solid;

	$db->executar($sql);
	$db->commit();
}

function enviarEmailSolicitacao( $dados ){

	global $db;

	$remetente = array('nome'=>REMETENTE_WORKFLOW_NOME, 'email'=>REMETENTE_WORKFLOW_EMAIL);

	if (!IS_PRODUCAO) {
		$dados['to'] = $db->pegaUm('SELECT usuemail FROM seguranca.usuario WHERE usucpf = \''.$_SESSION['usucpf'].'\'');
		enviar_email($remetente, $dados['to'], $dados['assunto'], $dados['mailBody'] );
	} else {
		enviar_email($remetente, $dados['to'], $dados['assunto'], $dados['mailBody']  );
	}
}

function wf_pos_retorna_solicitacao( $solid = NULL ){

	global $db;

	if($solid){
		$sql = "SELECT
					sol.solnome || replace(to_char(sol.usucpf::bigint, '000:000:000-00'), ':', '.') as nome,
					to_char(sol.soldatainclusao,'DD/MM/YYYY') as inclusao,
					usu.usuemail as to
				FROM
					evento.solicitacaodiaria sol
				INNER JOIN seguranca.usuario usu ON usu.usucpf = sol.solusucpf
				WHERE
					sol.solid = ".$solid;

		$dados = $db->pegaLinha($sql);
	}

	$dados['assunto']   = "[SIMEC] Solicitação retornada para 'Em solicitação' - Sistema de Solicitação de Diárias - Módulo Administrativo";
	$dados['mailBody']  = '
	Prezados Senhores, <br>
	<br>
	Informamos que a solicitação do Sr(a) "'.$dados['nome'].'" iniciada no dia '.$dados['inclusao'].',<br>
	retornou de \'Em Verificação\' para \'Em Solicitação\'.<br>
	<br>
	<br>
	<a href="http://simec.mec.gov.br">Clique Aqui para acessar o SIMEC.</a>
	<br>
	<br>
	Atenciosamente,<br>
	<br>
	<br>
	SIMEC<br>
	';

	if($dados['to']!=''){
		enviarEmailSolicitacao( $dados );
		return true;
	}else{
		return true;
	}
}

function wf_pos_retorna_verificacao( $solid = NULL ){

	global $db;

	if($solid){
		$sql = "SELECT
					sol.solnome || replace(to_char(sol.usucpf::bigint, '000:000:000-00'), ':', '.') as nome,
					to_char(sol.soldatainclusao,'DD/MM/YYYY') as inclusao,
					usu.usuemail as to
				FROM
					evento.solicitacaodiaria sol
				INNER JOIN seguranca.usuario usu ON usu.usucpf = sol.solusucpf
				WHERE
					sol.solid = ".$solid;

		$dados = $db->pegaLinha($sql);
	}

	$dados['assunto']   = "[SIMEC] Solicitação retornada para 'Em verificação' - Sistema de Solicitação de Diárias - Módulo Administrativo";
	$dados['mailBody']  = '
	Prezados Senhores, <br>
	<br>
	Informamos que a solicitação do Sr(a) "'.$dados['nome'].'" iniciada no dia '.$dados['inclusao'].',<br>
	retornou de \'Em Autorização\' para \'Em Verificação\'.<br>
	<br>
	<br>
	<a href="http://simec.mec.gov.br">Clique Aqui para acessar o SIMEC.</a>
	<br>
	<br>
	Atenciosamente,<br>
	<br>
	<br>
	SIMEC<br>
	';

	if($dados['to']!=''){
		enviarEmailSolicitacao( $dados );
		return true;
	}else{
		return true;
	}
}

function wf_pos_retorna_autorizacao( $solid = NULL ){

	global $db;

	if($solid){
		$sql = "SELECT
					sol.solnome || replace(to_char(sol.usucpf::bigint, '000:000:000-00'), ':', '.') as nome,
					to_char(sol.soldatainclusao,'DD/MM/YYYY') as inclusao,
					usu.usuemail as to
				FROM
					evento.solicitacaodiaria sol
				INNER JOIN seguranca.usuario usu ON usu.usucpf = sol.solusucpf
				WHERE
					sol.solid = ".$solid;

		$dados = $db->pegaLinha($sql);
	}

	$dados['assunto']   = "[SIMEC] Solicitação retornada para 'Em autorização' - Sistema de Solicitação de Diárias - Módulo Administrativo";
	$dados['mailBody']  = '
	Prezados Senhores, <br>
	<br>
	Informamos que a solicitação do Sr(a) "'.$dados['nome'].'" iniciada no dia '.$dados['inclusao'].',<br>
	retornou de \'Em Pagamento\' para \'Em Autorização\'.<br>
	<br>
	<br>
	<a href="http://simec.mec.gov.br">Clique Aqui para acessar o SIMEC.</a>
	<br>
	<br>
	Atenciosamente,<br>
	<br>
	<br>
	SIMEC<br>
	';

	if($dados['to']!=''){
		enviarEmailSolicitacao( $dados );
		return true;
	}else{
		return true;
	}
}


function wf_condicao_comite() {
	global $db;
	$existe_ar = $db->pegaUm("SELECT axpid FROM evento.anexoevento WHERE eveid='".$_SESSION['evento']['eveid']."' AND axestatus='A'");
	$existe_iv = $db->pegaUm("SELECT ievid FROM evento.itemevento  WHERE eveid='".$_SESSION['evento']['eveid']."' AND  ievstatus='A'");

	if($existe_iv && $existe_ar) return true;
	else return false;
}

    function cancelarEvento(){
	global $db;

       	if($_SESSION['evento']['eveid']){
            $sql = "
                DELETE FROM evento.itemconsumo WHERE eveid = {$_SESSION['evento']['eveid']};
                UPDATE evento.evento SET evestatus = 'I' WHERE eveid = {$_SESSION['evento']['eveid']};";
            $db->executar($sql);

            if($db->commit()){
                return true;
            }
	}
	return false;
    }

    function wf_verificaPrazoEnvioSecretaria(){
        global $db;

	$msg = eventoEnviaAnaliseComite();

	if($msg != 'OK'){
            return $msg;
	}

	if($_SESSION['evento']['eveid']){
            #verifica prazo
            $sql = "
                SELECT CASE 
                            WHEN evepublicoestimado <= 50 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 30 THEN 'false'
                            WHEN evepublicoestimado BETWEEN 51 AND 250 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 45 then 'false'
                            WHEN evepublicoestimado BETWEEN 251 AND 500 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 60 then 'false'
                            WHEN evepublicoestimado > 500 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 90 then 'false'
                            ELSE 'true'
                        END as prazo,
                        ungcod,
                        evecustoprevisto

                FROM evento.evento

                WHERE eveid = {$_SESSION['evento']['eveid']};
            ";
            $rs = $db->pegaLinha($sql);

            if($rs['prazo'] == 'true'){
                return 'O Evento deve estar fora do prazo para enviar para a Secretaria Executiva de Eventos';
            }

            $evedatainclusao = buscaDataInclusaoEvento();
            $data_nova_regra_evento = strtotime(DATA_NOVO_REGRA_EVENTO_NOVA_INFRA);

            if( $evedatainclusao < $data_nova_regra_evento ){
                //verifica saldo no contrato - LEGADO.
                $preid = $db->pegaUm("select preid from evento.pregaoevento_old where prestatus = 'A'");
                if($preid && $rs['ungcod']){
                    $urevalorsaldo = $db->pegaUm("select urevalorsaldo from evento.unidaderecurso_old where preid = $preid and ungcod = '".$rs['ungcod']."'");

                    if($rs['evecustoprevisto'] > $urevalorsaldo){
                        return 'Saldo insuficiente para esta Unidade Gestora!';
                    }
                }else{
                    return 'Não existe contrato para esta Unidade Gestora!';
                }
            }else{
                #BUSCA O CUSTO PREVISTO E EMPENHO DO EVENTO.
                $sql = "
                    SELECT 	trim(to_char(ev.evecustoprevisto, '999G999G999G990D99')) AS evecustoprevisto,
                            ev.emuid
                    FROM evento.evento AS ev
                    JOIN evento.empenho_unidade AS ep ON ep.emuid = ev.emuid
                    WHERE eveid = {$_SESSION['evento']['eveid']}
                ";
                $dadosEvento = $db->pegalinha($sql);
                $evecustoprevisto = desformata_valor( $dadosEvento['evecustoprevisto'] );

                if( $dadosEvento['emuid'] > 0 ){
                    #BUSCA O VALOR UTILIZADO NO EMPENHO
                    $sql = "SELECT trim(to_char(sum(empvalorutilizado), '999G999G999G990D99')) AS empvalorutilizado FROM evento.empenho_unidade WHERE empstatus = 'A' AND emuid = {$dadosEvento['emuid']};";
                    $valor_utilizado_empenho = desformata_valor( $db->pegaUm($sql) );

                    #BUSCA O SALDO INICIAL DO EMPENHO.
                    $sql = "SELECT trim(to_char(empsaldoinicontrato, '999G999G999G990D99')) FROM evento.empenho_unidade WHERE empstatus = 'A' AND emuid = {$dadosEvento['emuid']};";
                    $saldo_empenho = desformata_valor( $db->pegaUm($sql) );
                }

                #VERIFICA SE A SALDO DO EMPENHO SUFICIENTE PARA CUSTEAR EVENTO.
                if( $evecustoprevisto > ( $saldo_empenho - $valor_utilizado_empenho ) ){
                    return 'Não há saldo suficiente nesse empenho para custear este evento!';
                }
            }
            return true;
	}else{
            return 'Sessão expirou. Entre novamente no sistema.';
	}
    }

    function wf_verificaPrazoEnvioComite(){
        global $db;

        $perfis = pegaPerfilGeral();
        $msg = eventoEnviaAnaliseComite();

        if($msg != 'OK'){
            return $msg;
        }

        #verifica prazo
        if($_SESSION['evento']['eveid']){
            $sql = "
                SELECT  CASE
                            WHEN evepublicoestimado <= 50 AND DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 30 THEN 'false'
                            WHEN evepublicoestimado BETWEEN 51 AND 250 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 45 THEN 'false'
                            WHEN evepublicoestimado BETWEEN 251 AND 500 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 60 THEN 'false'
                            WHEN evepublicoestimado > 500 and DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) < 90 THEN 'false'
                            ELSE 'true'
                        END as prazo,
                        DATE_PART('days', (to_char(evedatainicio, 'YYYY-mm-dd'))::timestamp - NOW()) as dias,
                        ungcod,
                        evecustoprevisto
                FROM evento.evento

                WHERE eveid = {$_SESSION['evento']['eveid']};
            ";
            $rs = $db->pegaLinha($sql);

            if( in_array(EVENTO_PERFIL_SUPER_USUARIO, $perfis) ){

            }else{
                if(!$rs['prazo'] || $rs['prazo'] == 'false'){
                    return 'O Evento deve estar dentro do prazo para enviar para análise do comitê';
                }
            }

            $evedatainclusao = buscaDataInclusaoEvento();
            $data_nova_regra_evento = strtotime(DATA_NOVO_REGRA_EVENTO_NOVA_INFRA);

            if( $evedatainclusao < $data_nova_regra_evento ){
                #verifica saldo no contrato - LEGADO.
                $preid = $db->pegaUm("SELECT preid FROM evento.pregaoevento_old WHERE prestatus = 'A'");
                if($preid && $rs['ungcod']){
                    $urevalorsaldo = $db->pegaUm(" SELECT urevalorsaldo FROM evento.unidaderecurso_old WHERE preid = {$preid} and ungcod = '{$rs['ungcod']}';" );

                    if($rs['evecustoprevisto'] > $urevalorsaldo){
                        return 'Saldo insuficiente para esta Unidade Gestora!';
                    }
                }else{
                    return 'Não existe contrato para esta Unidade Gestora!';
                }
            }else{
                #BUSCA O CUSTO PREVISTO E EMPENHO DO EVENTO.
                $sql = "
                    SELECT  trim(to_char(ev.evecustoprevisto, '999G999G999G990D99')) AS evecustoprevisto,
                            ev.emuid
                    FROM evento.evento AS ev
                    JOIN evento.empenho_unidade AS ep ON ep.emuid = ev.emuid
                    WHERE eveid = {$_SESSION['evento']['eveid']}
                ";
                $dadosEvento = $db->pegalinha($sql);
                $evecustoprevisto = desformata_valor( $dadosEvento['evecustoprevisto'] );

                if( $dadosEvento['emuid'] > 0 ){
                    #BUSCA O VALOR UTILIZADO NO EMPENHO
                    $sql = "SELECT trim(to_char(sum(empvalorutilizado), '999G999G999G990D99')) AS empvalorutilizado FROM evento.empenho_unidade WHERE empstatus = 'A' AND emuid = {$dadosEvento['emuid']};";
                    $valor_utilizado_empenho = desformata_valor( $db->pegaUm($sql) );

                    #BUSCA O SALDO INICIAL DO EMPENHO.
                    $sql = "SELECT trim(to_char(empsaldoinicontrato, '999G999G999G990D99')) FROM evento.empenho_unidade WHERE empstatus = 'A' AND emuid = {$dadosEvento['emuid']};";
                    $saldo_empenho = desformata_valor( $db->pegaUm($sql) );
                }

                #VERIFICA SE A SALDO DO EMPENHO SUFICIENTE PARA CUSTEAR EVENTO.
                if( $evecustoprevisto > ( $saldo_empenho - $valor_utilizado_empenho ) ){
                    return 'Não há saldo suficiente nesse empenho para custear este evento!';
                }
            }
            return true;
        }else{
            return 'Sessão expirou. Entre novamente no sistema.';
        }
    }

    function wf_aprovaAdReferendum(){
	global $db;

	if($_SESSION['evento']['eveid']){
            $sql = "update evento.evento set eveurgente = 't' where eveid = ".$_SESSION['evento']['eveid'];
            $db->executar($sql);
            if($db->commit()){
                return true;
            }
	}
	return false;
    }


function mascaraglobal2($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}


#LEGADO - CADASTRO DE EMPENHO - FUNÇÃO PARA BUSCAR OS DADOS DO EMPENHO - DATA DE 09/04/2013
function carregarEmpenho($dados){
    global $db;

    extract($dados);
    /* Status agora é definido pelo usuario - Regra Nova
     * 20/02/2014
    * Pedido por: Juvenal Feito por: Eduardo
    * */
    if($emuid){
        $sql = "SELECT * FROM evento.empenho_unidade_old WHERE emuid = ".$emuid;
        $rs = $db->pegaLinha($sql);
        echo $rs['emuid']."||".$rs['ungcod']."||".$rs['empnumero']."||".$rs['empdescricao']."||".$rs['empnumeropi']."||".$rs['empano']."||".$rs['empstatus'];
        exit;
    }
}



#LEGADO - CADASTRO DE EMPENHO - FUNÇÃO PARA CADASTRAR / ATUALIZAR EMPENHOS - DATA DE 09/04/2013
function salvarEmpenho($dados){
    global $db;

    extract($dados);


    /* Status agora é definido pelo usuario - Regra Nova
     * 20/02/2014
     * Pedido por: Juvenal Feito por: Eduardo
     * */
    if( $ungcod != '' && $emuid == '' ){
        $sql = "
            INSERT INTO evento.empenho_unidade_old(
                    ungcod,
                    empnumero,
                    empdescricao,
                    empnumeropi,
                    empano,
                    empstatus
                )VALUES (
                    '{$ungcod}',
                    '{$empnumero}',
                    '".addslashes( $empdescricao )."',
                    '".$empnumeropi."',
                    '".$empano."',
                   	'$empstatus'
                ) RETURNING emuid;
        ";
        $msg = "Dados Gravados com sucesso.";

        $emuid = $db->executar($sql);

    }elseif( $ungcod != '' && $emuid > 0 ){
        $sql = "
            UPDATE evento.empenho_unidade_old
                SET empnumero       = '{$empnumero}',
                    empdescricao    = '".addslashes( $empdescricao )."',
                    empnumeropi     = '".$empnumeropi."',
                    empano          = '".$empano."',
					empstatus		= '$empstatus'
                WHERE emuid = ".$emuid." and ungcod = '".$ungcod."' RETURNING emuid;
        ";
        $msg = "Dados Atualizados com sucesso.";

        $emuid = $db->executar($sql);
    }

    if( $emuid > 0 ){
        $db->commit();
        $db->sucesso('principal/CadEmpenhos', '&acao=A&ungcod='.$ungcod.'&form_pesquisa=empenho', $msg);
    }
}

##LEGADO - CADASTRO DE EMPENHO - FUNÇÃO PARA ATUALIZAR STATUS PARA "I" OS DADOS DO EMPENHOS - DATA DE 09/04/2013
function exclirEmpenho($dados){
    global $db;

    extract($dados);

    if($ungcod != '' && $emuid > 0){
        $sql = "
            UPDATE evento.empenho_unidade_old
                SET empstatus = 'I'
            WHERE emuid = {$emuid} and ungcod = '{$ungcod}' RETURNING emuid;
        ";
        $emuid = $db->executar($sql);
    }

    if( $emuid > 0 ){
        $db->commit();
        $db->sucesso('principal/CadEmpenhos', '&acao=A&ungcod='.$ungcod.'&form_pesquisa=empenho', 'Exclusão realizada com sucesso');
    }
}

#ORDER DE SERVIÇO - FUNÇÃO PARA VALIDAÇÃO DA DATA, A DATA DA GERAÇÃO DE OS. NÃO PODE SER MAIOR QUE A DATA ATUAL- DATA DE 09/04/2013
function validaDataEvento(){
    global $db;

    $perfis = pegaPerfilGeral();

    $sql = "
        SELECT  ev.eveid,
                ev.evedatainicio,
                ev.evedatafim
        FROM evento.evento AS ev
        WHERE ev.eveid = {$_SESSION['evento']['eveid']}
    ";
    $dados = $db->pegaLinha($sql);

    $dataHoje = strtotime("now");
    $dataInicio = strtotime($dados['evedatainicio']);

    if( $dataHoje < $dataInicio ){
        $msg = "ok";
    }else{
        $msg = "erro";

        if( in_array(EVENTO_PERFIL_SAA, $perfis) || in_array(EVENTO_PERFIL_SUPER_USUARIO, $perfis) ){
            $msg = "saa";
        }
    }
    echo $msg;
}


    #DOCUMENTO DE PAGAMENTO - VERIFICA SE FOI GERADO O DOCUMETO DE PAGAMENTO PARA HABILITAR O WORKFLOW
    function validaDocPagamento(){
        global $db;

        $sql = "
            SELECT dpaid FROM evento.documentopagamento WHERE dpavalor IS NOT NULL AND dpanumero IS NOT NULL AND eveid = {$_SESSION['evento']['eveid']};
        ";
        $dpaid = $db->pegaUm($sql);

        if( $dpaid > 0 ){
            return true;
        }else{
            return false;
        }
    }

function condicaoGeraqrOS() {
	global $db;

	$eveid = $_SESSION['evento']['eveid'];
	$datahoje = date('Y-m-d');

	if(!$eveid) return false;

	$sql = "SELECT
				evedatainicio,
				evenumeroprocesso
			FROM
				evento.evento
			WHERE
				eveid = {$eveid}";
	$dados = $db->pegaLinha($sql);

	if($dados['evedatainicio'] =='' || $dados['evenumeroprocesso']==''){
		return false;
	} elseif( strtotime($dados['evedatainicio']) < strtotime($datahoje)){
		return false;
	} else {
		return true;
	}
}


#--------------------------------------------------- FUNÇÕES DE EVENTOS NOVOS CADASTROS ---------------------------------------------------#

#editarPregao
#salvarDadosPregao

    /**
     * functionName atualizaCombosAssoc
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $CEP cep
     * @return string json "array" com dados referentes ao endereço.
     *
     * @version v1
    */
    function atualizaCombosAssoc( $dados ){
        global $db;

        $emuid = $dados['emuid'];

        $sql = "
            SELECT  precodpregao ||' - '|| prenumprocesso AS predescpregao,
                    un.ungcod,
                    un.ungdsc,
                    ur.ureid,
                    cp.coenumcontrato,
                    cp.coefimvig
            FROM evento.empenho_unidade AS eu

            JOIN evento.contratopregao AS cp ON cp.coeid = eu.coeid
            JOIN evento.unidaderecurso AS ur ON ur.ureid = eu.ureid
            JOIN evento.pregaoevento AS pe ON pe.preid = eu.preid
            JOIN public.unidadegestora AS un ON un.ungcod = ur.ungcod

            WHERE emuid = {$emuid}
        ";
        $dados = $db->pegaLinha($sql);

        $coefimvig  = strtotime($dados['coefimvig']);
        $hoje       = strtotime(date('Y-m-d'));

        if( $hoje <= $coefimvig  ){
            $vigencia = 'S';
        }else{
            $vigencia = 'N';
        }

        if($dados != ''){
            foreach($dados as $key => $resposta){
                $dados[$key] = iconv("ISO-8859-1", "UTF-8", $resposta );
            }
            $dados['vigencia'] = $vigencia;
        }else{
            foreach($dados as $key => $resposta){
                $dados[$key] = '';
            }
        }
        echo simec_json_encode( $dados );
        die;
    }

/**
     * functionName atualizaCombosContrato
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $emuid id da tabela de empenho.
     * @return string json "array" com dados referentes ao endereço.
     *
     * @version v1
    */
    function atualizaCombosContrato( $dados ){
        global $db;

        $preid = $dados['preid'];
        $sql = "
            SELECT  coeid AS codigo,
                    coenumcontrato ||' - '|| coecnpj ||' - '|| coerazaosocial AS descricao
            FROM evento.contratopregao
            WHERE preid = {$preid}
            ORDER BY descricao
        ";
        $db->monta_combo('coeid', $sql, 'S', "Selecione...", '', '', '', 500, 'S', 'coeid', '', $preid, 'Contrato', '', 'chosen-select');
        die;
    }

    /**
     * functionName atualizaComboEstado
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $emuid id da tabela de empenho.
     * @return string json "array" com dados referentes ao endereço.
     *
     * @version v1
    */
    function atualizaComboEstado( $dados ){
        global $db;

        $emuid = $dados['emuid'];

        $sql = "
            SELECT  estuf as codigo,
                    estdescricao as descricao
            FROM territorios.estado
            WHERE estuf IN (
                SELECT g.gruuf
                FROM evento.empenho_unidade AS eu
                JOIN evento.contratopregao AS cp ON cp.coeid = eu.coeid
                JOIN evento.itemvalor AS iv ON iv.coeid = cp.coeid
                JOIN evento.grupoestado AS g ON g.gruid = iv.gruid
                WHERE eu.emuid = {$emuid}
                GROUP BY g.gruuf
            )
            ORDER BY estdescricao
        ";
        echo $db->monta_combo('estuf', $sql, $somenteLeituraUnidade, "Selecione...", 'filtraMunicipio', '', '', '200', 'S', 'estuf');
        die;
    }

    /**
     * functionName buscaDataInclusaoEvento
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $eveid id da tabela evento.evento, é o identificador do evento.
     * @return string "data" data de inclusão do evento.
     *
     * @version v1
    */
    function buscaDataInclusaoEvento(){
        global $db;

        $eveid = $_SESSION['evento']['eveid'];
        if( $eveid != '' ){
            $sql = "
                SELECT evedatainclusao FROM evento.evento WHERE eveid = {$eveid};
            ";
            $evedatainclusao = strtotime( $db->pegaUm($sql) );
        }else{
            $evedatainclusao = strtotime(date("Y-m-d"));
        }
        return $evedatainclusao;
    }

    /**
     * functionName buscaDadosCadEvento
     *
     * @author Luciano F. Ribeiro
     *
     * @info faz o direcioanmento as tabelas "empenho, unidade pregao etc" de acordo com a data de inclusao do evento.
     * @param string $eveid id da tabela evento.evento, é o identificador do evento.
     * @return string "array" com dados referentes ao ao evento.
     *
     * @version v1
    */
    function buscaDadosCadEvento( $eveid ){
        global $db;

        #TRATAMENTO PARA USO DO CADASTRATO DE EVENTOS COM AS REGRAS ANTIGAS "LEGADO".
        if( $eveid != ''){

            #BUSCA A DATA DE INCLUSAO DO EVENTO.
            $evedatainclusao = buscaDataInclusaoEvento();
            $data_nova_regra_evento = strtotime(DATA_NOVO_REGRA_EVENTO_NOVA_INFRA);

            if( $evedatainclusao < $data_nova_regra_evento ){
                #PASSIVOS.
                $JOIN = "
                    LEFT JOIN evento.tipoevento AS te ON te.tpeid  = ev.tpeid
                    LEFT JOIN public.unidadegestora AS u ON ev.ungcod = u.ungcod
                    LEFT JOIN seguranca.usuario AS us ON us.usucpf = ev.usucpf
                    LEFT JOIN evento.avaliacaoevento aval ON aval.eveid = ev.eveid

                    LEFT JOIN evento.unidaderecurso_old AS ur ON ur.ungcod = ev.ungcod
                    LEFT JOIN territorios.municipio mun ON mun.muncod = ev.muncod
                    LEFT JOIN evento.ordemservico ord ON ord.eveid = ev.eveid
                    LEFT JOIN evento.empenho_unidade_old ep ON ep.emuid = cast(ord.oseempenho as integer)
                    LEFT JOIN evento.reuniaocomite AS re ON re.rcoid = ord.rcoid
                ";
                $campos = "
                    ev.ureid,
                    u.ungdsc,
                    ep.empnumero as numero_empenho,
                ";
            }else{
                #NOVOS
                $JOIN = "
                    LEFT JOIN territorios.municipio mun ON mun.muncod = ev.muncod
                    LEFT JOIN evento.tipoevento AS te ON te.tpeid  = ev.tpeid
                    LEFT JOIN public.unidadegestora AS u ON ev.ungcod = u.ungcod
                    LEFT JOIN seguranca.usuario AS us ON us.usucpf = ev.usucpf
                    LEFT JOIN evento.avaliacaoevento aval ON aval.eveid = ev.eveid

                    JOIN evento.empenho_unidade AS eu ON eu.emuid = ev.emuid
                    JOIN evento.contratopregao AS cp ON cp.coeid = eu.coeid
                    JOIN evento.pregaoevento AS p ON p.preid = eu.preid
                    JOIN evento.unidaderecurso AS un ON un.ureid = eu.ureid
                    JOIN public.unidadegestora AS ug ON ug.ungcod = un.ungcod

                    LEFT JOIN evento.ordemservico AS ord ON ord.eveid = ev.eveid
                    LEFT JOIN evento.reuniaocomite AS re ON re.rcoid = ord.rcoid
                ";
                $campos = "
                    eu.ureid,
                    precodpregao ||' - '|| prenumprocesso AS predescpregao,
                    eu.empnumero AS numero_empenho,
                    ug.ungdsc,
                    cp.coeid,
                    cp.coenumcontrato,
                ";
            }
        }else{
            $JOIN = "
                LEFT JOIN evento.tipoevento AS te ON te.tpeid  = ev.tpeid
                LEFT JOIN public.unidadegestora AS u ON ev.ungcod = u.ungcod
                LEFT JOIN seguranca.usuario AS us ON us.usucpf = ev.usucpf
                LEFT JOIN evento.avaliacaoevento aval ON aval.eveid = ev.eveid

                JOIN evento.empenho_unidade AS eu ON eu.emuid = ev.emuid
                JOIN evento.contratopregao AS cp ON cp.coeid = eu.coeid
                JOIN evento.pregaoevento AS p ON p.preid = eu.preid
                JOIN evento.unidaderecurso AS un ON un.ureid = eu.ureid
                JOIN public.unidadegestora AS ug ON ug.ungcod = un.ungcod
            ";
            $campos = "
                eu.ureid,
                precodpregao ||' - '|| prenumprocesso AS predescpregao,
                eu.empnumero AS numero_empenho,
                ug.ungdsc,
                cp.coenumcontrato,
            ";
        }

        $sql = "
            SELECT  ev.emuid,
                    {$campos}
                    ev.ungcod,
                    ev.tpeid,
                    ev.evetitulo,
                    to_char( ev.evedatainicio, 'DD/MM/YYYY') AS evedatainicio,
                    to_char( ev.evedatafim, 'DD/MM/YYYY' ) AS evedatafim,
                    ev.eveemail,
                    ev.everespnome,
                    ev.everesptelefone,
                    ev.evecustoprevisto,
                    ev.evepublicoestimado,
                    ev.evequantidadedias,

                    --MUNICIPIO
                    ev.muncod, mun.mundescricao,
                    ev.estuf,
                    ev.sevid,
                    ev.eveqtdpassagemaerea,
                    us.usunome,
                    ev.docid,
                    ev.eveurgente,
                    aval.aevid,
                    ev.endid,
                    ev.evedatainclusao::date AS evedatainclusao,
                    ev.evelocal,
                    ev.evenumeroprocesso,
                    ev.evedemandante,
                    ev.evecargodemandante,
                    ev.evenumeropi,

                    --DADOS DA OS.
                    oseid,
                    osenumeroos,
                    to_char(osedataemissaoos::date,'DD/MM/YYYY') AS osedataemissaoos,
                    to_char(osedatainiciofinal::date,'DD/MM/YYYY') as osedatainiciofinal,
                    to_char(osedatafimfinal::date,'DD/MM/YYYY') as osedatafimfinal,
                    osecustofinal, oseobsos, osecnpj, oserazaosocial,
                    oseproposta, osecodpregao, oseordenador, oseempenho,
                    osetipoordenador, ureordenador, ureordenadorsub, osenumcontrato, ord.rcoid, re.rcodescricao
            FROM evento.evento AS ev

            {$JOIN}

            WHERE ev.eveid = {$_SESSION['evento']['eveid']}
        ";
        return $rsDadosEvento = $db->pegaLinha($sql);
    }

    /**
     * functionName dowloadAnexo
     *
     * @author Luciano F. Ribeiro
     *
     * @param array $dados é usado o id da pergunta.
     * @return o download do arquivo".
     *
     * @version v1
    */
    function dowloadAnexo( $dados ){

        $arqid = $dados['arqid'];

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        if ( $arqid ){
            $file = new FilesSimec('jsut_os_extraordinario', $campos, 'evento');
            $file->getDownloadArquivo( $arqid );
        }
    }

    /**
     * functionName excluirAnexo
     *
     * @author Luciano F. Ribeiro
     *
     * @param array $dados é usado o id da pergunta.
     * @return exclusão logica e fisica do arquivo.
     *
     * @version v1
    */
    function excluirAnexo( $dados ) {
        global $db;

        $arqid = $dados['arqid'];
        $prgid = $dados['prgid'];

        //include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        if ($arqid != ''){
            $sql = " UPDATE maismedicomec.arquivoresposta SET arqstatus = 'I' WHERE arqid = {$arqid} ";
        }

        if( $db->executar($sql) ){
            $file = new FilesSimec('arquivoresposta', $campos, 'maismedicomec');
            $file->excluiArquivoFisico( $arqid );

            $db->commit();
            $db->sucesso('principal/instrumentoavaliacao/lista_grid_arquivos_anexo', '&acao=A&prgid='.$prgid);
        }
    }

    /**
     * functionName editarPregao
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $CEP cep
     * @return string json "array" com dados referentes ao endereço.
     *
     * @version v1
    */
    function editarContrato( $coeid ){
        global $db;

        $sql = "
            SELECT  coeid,
                    preid,
                    coenumcontrato,
                    to_char(coeiniciovig, 'DD/MM/YYYY') AS coeiniciovig,
                    to_char(coefimvig, 'DD/MM/YYYY') AS coefimvig,
                    to_char(coeiniciovig, 'DD/MM/YYYY') AS coeiniciovig,
                    to_char(coefimvig, 'DD/MM/YYYY') AS coefimvig,
                    to_char(coevalorcontratado, '999G999G999G990D99') AS coevalorcontratado,
                    to_char(coevalorempenhado, '999G999G999G990D99') AS coevalorempenhado,
                    to_char(coesaldoinicontrato, '999G999G999G990D99') AS coesaldoinicontrato,
                    to_char(coevalorutilizado, '999G999G999G990D99') AS coevalorutilizado,
                    to_char(coesaldoiniexercicio, '999G999G999G990D99') AS coesaldoiniexercicio,
                    to_char(coesaldofimcontrato, '999G999G999G990D99') AS coesaldofimcontrato,
                    replace(to_char(cast(coecnpj as bigint), '00:000:000/0000-00'), ':', '.') as coecnpj,
                    coenumprocesso,
                    coerazaosocial
            FROM evento.contratopregao

            WHERE coeid = '{$coeid}'
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            return $dados;
        }
    }

    /**
     * functionName editarEmpenho
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $CEP cep
     * @return string json "array" com dados referentes ao endereço.
     *
     * @version v1
    */
    function editarEmpenho( $dados ){
        global $db;

        $emuid = $dados['emuid'];

        $sql = "
            SELECT  emuid,
                    preid,
                    coeid,
                    ureid,
                    empnumero,
                    empdescricao,
                    empnumeropi,
                    empano,
                    trim(to_char(empsaldoinicontrato, '999G999G999G990D99') ) AS empsaldoinicontrato,
                    trim(to_char(empvalorutilizado, '999G999G999G990D99') ) AS empvalorutilizado
            FROM evento.empenho_unidade WHERE emuid = '{$emuid}'
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            foreach($dados as $key => $resposta){
                $dados[$key] = iconv("ISO-8859-1", "UTF-8", $resposta );
            }
        }else{
            foreach($dados as $key => $resposta){
                $dados[$key] = '';
            }
        }
        echo simec_json_encode( $dados );
        die;
    }

    /**
     * functionName editarPregao
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $CEP cep
     * @return string json "array" com dados referentes ao endereço.
     *
     * @version v1
    */
    function editarPregao( $preid ){
        global $db;
        
        $sql = "
            SELECT * FROM evento.pregaoevento WHERE preid = '{$preid}'
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            return $dados;
        }
    }

    /**
     * functionName editarUndRecurso
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function editarUndRecurso( $ureid ){
        global $db;
        
        $sql = "
            SELECT  *,
                    trim(to_char(uresaldoinicontrato, '999G999G999G990D99') ) AS uresaldoinicontrato,
                    trim(to_char(urevalorutilizado, '999G999G999G990D99') ) AS urevalorutilizado

            FROM evento.unidaderecurso WHERE ureid = {$ureid}
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            return $dados;
        }
    }

    /**
     * functionName excluirContrato
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function excluirContrato( $dados ){
        global $db;

        $coeid = $dados['coeid'];

        $sql = "
            UPDATE evento.contratopregao SET coestatus = 'I' WHERE coeid = '{$coeid}' RETURNING coeid;
        ";
        $up_coeid = $db->pegaLinha($sql);

        if( $up_coeid > 0){
            $db->commit();
            $db->sucesso( 'principal/eventos/cad_eve_contrato', '', "A operação foi realizada com sucesso!");
        }else{
            $db->rollback();
            $db->sucesso( 'principal/eventos/cad_eve_contrato', '', "Não foi possível realizar a operação, tente novamente mais tarde!");
        }
    }


    /**
     * functionName exclirDadosEmpenho
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v2
    */
    function exclirDadosEmpenho( $dados ){
        global $db;

        $emuid = $dados['emuid'];
        $ureid = $dados['ureid'];

        if( $emuid > 0 ){
            $sql = "
                DELETE FROM evento.empenho_unidade WHERE emuid = {$emuid} RETURNING emuid;
            ";
            $emuid = $db->pegaUm($sql);
        }

        if( $emuid > 0 ){
            $db->commit();
            $db->sucesso('principal/eventos/cad_eve_empenho', '&ureid='.$ureid, 'Exclusão realizada com sucesso');
        }else{
            $db->rollback();
            $db->sucesso( 'principal/eventos/cad_eve_empenho', '', "Não foi possível realizar a operação, tente novamente mais tarde!");
        }
    }

    /**
     * functionName excluirPregao
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function excluirPregao( $dados ){
        global $db;

        $preid = $dados['preid'];
        
        $sql = "
            UPDATE evento.pregaoevento SET prestatus = 'I' WHERE preid = '{$preid}' RETURNING preid;
        ";
        $up_preid = $db->pegaLinha($sql);

        if( $up_preid > 0){
            $db->commit();
            $db->sucesso( 'principal/eventos/cad_eve_pregao', '', "A operação foi realizada com sucesso!");
        }else{
            $db->rollback();
            $db->sucesso( 'principal/eventos/cad_eve_pregao', '', "Não foi possível realizar a operação, tente novamente mais tarde!");
        }
    }

    /**
     * functionName excluirUndRecurso
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function excluirUndRecurso( $dados ){
        global $db;

        $ureid = $dados['ureid'];

        if( $ureid != '' ){
            $sql = "
                SELECT emuid FROM evento.empenho_unidade WHERE ureid = {$ureid} GROUP BY emuid;
            ";
            $emuid = $db->pegaUm($sql);

            if( $emuid == '' ){
                $sql = "
                    DELETE FROM evento.unidaderecurso WHERE ureid = '{$ureid}' RETURNING ureid;
                ";
                $ureid = $db->pegaLinha($sql);
                $e_usado = 'N';
            }else{
                $e_usado = 'S';
                $msg = "Unidade é usada em algum empenho e não pode ser Deletada é necessário apagar os empenhos relacionados a ela!";
            }
        }

        if( $ureid > 0 && $e_usado == 'N' ){
            $db->commit();
            $db->sucesso( 'principal/eventos/cad_eve_und_recurso', '', "A operação foi realizada com sucesso!");
        }else{
            $msg = $e_usado == 'S' ? "Unidade é usada em algum empenho e não pode ser Deletada é necessário apagar os empenhos relacionados a ela!" : "Não foi possível realizar a operação, tente novamente mais tarde!";
            $db->rollback();
            $db->sucesso( 'principal/eventos/cad_eve_und_recurso', '', "{$msg}");
        }
    }

    function excluirItemEvento( $itvid ){
        global $db;

        $eveid = $_SESSION['evento']['eveid'];

        if( $eveid != '' ){
            #BUSCA VALOR PREVISTO PARA O EVENTO.
            $sql = "
                SELECT evecustoprevisto FROM evento.evento WHERE eveid = {$eveid};
            ";
            $evecustoprevisto = $db->pegaUm($sql);

            #BUSCA VALOR TOTAL DO ITEM USADO QUE SERA DELETADO.
            $sql = "
                SELECT ( sum(itcquantidade) * itcvalor ) AS total_iten
                FROM evento.itemconsumo
                WHERE itvid = {$itvid} AND eveid = {$eveid}
                GROUP BY itcvalor
            ";
            $total_iten = $db->pegaUm($sql);
        }

        if( $evecustoprevisto > 0 && $total_iten > 0 ){
            $atual_valor_previsto = $evecustoprevisto - $total_iten;
            #ATUALIZA O SALDO PREVISTO DO EVENTO.
            $sql = " UPDATE evento.evento SET evecustoprevisto = '{$atual_valor_previsto}' WHERE eveid = {$eveid}; ";

            if( $db->executar($sql) ) {
                $sql = "
                    DELETE FROM evento.itemconsumo where itvid = {$itvid} AND eveid = {$eveid} RETURNING itcid;
                ";
                $itcid = $db->pegaUm($sql);
            }
        }

        if( $itcid > 0 ){
            $db->commit();
            $db->sucesso( 'principal/eventos/cad_eve_infra', '', "A operação foi realizada com sucesso!");
        }else{
            $db->rollback();
            $db->sucesso( 'principal/eventos/cad_eve_infra', '', "Não foi possível realizar a operação, tente novamente mais tarde!");
        }
    }


    /**
     * functionName salvarDadosPregao
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function salvarDadosPregao( $dados ){
        global $db;

        $preid = $dados['preid'];

        $prenumprocesso = strtoupper( trim($dados['prenumprocesso']) );
        $precodpregao   = strtoupper( trim($dados['precodpregao']) );
        $predescpregao  = trim($dados['predescpregao']);

        if( $preid == '' ){
            $sql = "
                INSERT INTO evento.pregaoevento(
                        precodpregao, predescpregao, prenumprocesso, prestatus
                    )VALUES(
                        '{$precodpregao}', '{$predescpregao}', '{$prenumprocesso}', 'A'
                ) RETURNING preid;
            ";
        }else{
            $sql = "
                UPDATE evento.pregaoevento
                    SET precodpregao    = '{$precodpregao}',
                        predescpregao   = '{$predescpregao}',
                        prenumprocesso  = '{$prenumprocesso}'
                WHERE preid = {$preid} RETURNING preid;
            ";
        }
        $preid = $db->pegaUm($sql);

        if( $preid > 0){
            $db->commit();
            $db->sucesso( 'principal/eventos/cad_eve_pregao', '', "A operação foi realizada com sucesso!");
        }else{
            $db->rollback();
            $db->sucesso( 'principal/eventos/cad_eve_pregao', '', "Não foi possível realizar a operação, tente novamente mais tarde!");
        }
    }

    /**
     * functionName salvarDadosCadastro
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function salvarDadosContrato( $dados ){
        global $db;

        $coeid                  = trim( $dados['coeid'] );
        $preid                  = trim( $dados['preid'] );

        $coenumcontrato         = "'".strtoupper( $dados['coenumcontrato'] )."'";

        $coeiniciovig           = "'".formata_data_sql( $dados['coeiniciovig'] )."'";
        $coefimvig              = "'".formata_data_sql( $dados['coefimvig'] )."'";

        $coevalorcontratado     = $dados['coevalorcontratado'] != '' ? "'".desformata_valor( $dados['coevalorcontratado'] )."'" : 'NULL';
        $coevalorempenhado      = $dados['coevalorempenhado'] != '' ? "'".desformata_valor(  $dados['coevalorempenhado'] )."'" : 'NULL';

        $coesaldoinicontrato    = "'".desformata_valor( $dados['coesaldoinicontrato'] )."'";

        $coenumprocesso         = "'".strtoupper( trim( $dados['coenumprocesso'] ) )."'";
        $coecnpj                = "'".trim( str_replace( array('/', '.', '-'), "", $dados['coecnpj']) )."'";
        $coerazaosocial         = "'".trim( $dados['coerazaosocial'] )."'";

        if( $coeid == '' ){
            $sql = "
                INSERT INTO evento.contratopregao(
                    preid,
                    coenumcontrato,
                    coeiniciovig,
                    coefimvig,
                    coevalorcontratado,
                    coevalorempenhado,
                    coesaldoinicontrato,
                    coenumprocesso,
                    coecnpj,
                    coerazaosocial
                )VALUES(
                    {$preid},
                    {$coenumcontrato},
                    {$coeiniciovig},
                    {$coefimvig},
                    {$coevalorcontratado},
                    {$coevalorempenhado},
                    {$coesaldoinicontrato},
                    {$coenumprocesso},
                    {$coecnpj},
                    {$coerazaosocial}
                ) RETURNING coeid;
            ";
        }else{
            $sql = "
                UPDATE evento.contratopregao
                    SET preid               = {$preid},
                        coenumcontrato      = {$coenumcontrato},
                        coeiniciovig        = {$coeiniciovig},
                        coefimvig           = {$coefimvig},
                        coevalorcontratado  = {$coevalorcontratado},
                        coevalorempenhado   = {$coevalorempenhado},
                        coesaldoinicontrato = {$coesaldoinicontrato},
                        coenumprocesso      = {$coenumprocesso},
                        coecnpj             = {$coecnpj},
                        coerazaosocial      = {$coerazaosocial}
                WHERE coeid = {$coeid} RETURNING coeid;
            ";
        }
        $coeid = $db->pegaUm($sql);

        if( $coeid > 0){
            $db->commit();
            $db->sucesso( 'principal/eventos/cad_eve_contrato', '', "A operação foi realizada com sucesso!");
        }else{
            $db->rollback();
            $db->sucesso( 'principal/eventos/cad_eve_contrato', '', "Não foi possível realizar a operação, tente novamente mais tarde!");
        }
    }


    /**
     * functionName salvarDadosEmpenho
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function salvarDadosEmpenho($dados){
        global $db;

        $emuid                  = $dados['emuid'];
        $preid                  = $dados['preid'];
        $coeid                  = $dados['coeid'];
        $ureid                  = $dados['ureid'];

        $empdescricao           = trim( addslashes( $dados['empdescricao'] ) );
        $empnumero              = $dados['empnumero'];
        $empnumeropi            = $dados['empnumeropi'];
        $empano                 = $dados['empano'];
        $empsaldoinicontrato    = desformata_valor( $dados['empsaldoinicontrato'] );
        $empstatus              = $dados['empstatus'];

        #BUSCA SALDO E VALOR TOTAL.
        if( $ureid != '' ){
            $sql = "
                SELECT uresaldoinicontrato FROM evento.unidaderecurso WHERE ureid = {$ureid};
            ";
            $uresaldoinicontrato = desformata_valor( $db->pegaUm($sql) );

            $sql = "
                SELECT SUM(empsaldoinicontrato) AS total_empenho FROM evento.empenho_unidade WHERE ureid = {$ureid};
            ";
            $total_empenho = desformata_valor( $db->pegaUm($sql) );
        }

        if( $empsaldoinicontrato <= ( $uresaldoinicontrato - $total_empenho ) ){
            if( $emuid == '' ){
                $sql = "
                    INSERT INTO evento.empenho_unidade(
                            ureid,
                            preid,
                            coeid,
                            empnumero,
                            empdescricao,
                            empnumeropi,
                            empano,
                            empsaldoinicontrato,
                            empstatus
                        )VALUES (
                            {$ureid},
                            {$preid},
                            {$coeid},
                            '{$empnumero}',
                            '{$empdescricao}',
                            '{$empnumeropi}',
                            '{$empano}',
                            '{$empsaldoinicontrato}',
                            '{$empstatus}'
                        ) RETURNING emuid;
                ";
                $msg = "Dados Inseridos com sucesso!";
            }else{
                $sql = "
                    UPDATE evento.empenho_unidade
                        SET empnumero           = '{$empnumero}',
                            empdescricao        = '{$empdescricao}',
                            empnumeropi         = '{$empnumeropi}',
                            empano              = '{$empano}',
                            empsaldoinicontrato = '{$empsaldoinicontrato}',
                            empstatus           = '{$empstatus}'
                        WHERE emuid = {$emuid} RETURNING emuid;
                ";
                $msg = "Dados Atualizados com sucesso.";
            }
            $emuid = $db->pegaUm($sql);
            $msg = "Operação realizada com sucesso!";
        }else{
            if( $emuid == '' ){
                $sql = "
                    UPDATE evento.empenho_unidade
                        SET empnumero           = '{$empnumero}',
                            empdescricao        = '{$empdescricao}',
                            empnumeropi         = '{$empnumeropi}',
                            empano              = '{$empano}',
                            empstatus           = '{$empstatus}'
                        WHERE emuid = {$emuid} RETURNING emuid;
                ";
                $emuid = $db->pegaUm($sql);
            }
            $erro_saldo = 'S';
            $msg = "Dados descritivos desse Empenho foram atualizados com sucesso.";
        }

        if( $emuid > 0 ){
            $db->commit();
            $db->sucesso( 'principal/eventos/cad_eve_empenho', '&ureid='.$ureid, $msg);
        }else{
            $msg = $erro_saldo == 'S' ? "O saldo dessa Unidade não é sufuciente para custear esse Empenho" : "Não foi possível realizar a operação, tente novamente mais tarde!";
            $db->rollback();
            $db->sucesso( 'principal/eventos/cad_eve_empenho', '&ureid='.$ureid, "{$msg}");
        }
    }

    /**
     * functionName salvarDadosUndRecurso
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function salvarDadosUndRecurso( $dados ){
        global $db;

        $ureid = $dados['ureid'];
        $coeid = $dados['coeid'];
        $preid = $dados['preid'];
        $ungcod = $dados['ungcod'];
        $ureordenador = $dados['ureordenador'];
        $ureordenadorsub = $dados['ureordenadorsub'];
        $uresaldoinicontrato = desformata_valor( $dados['uresaldoinicontrato'] );

        $sql = "
            SELECT trim(to_char(coesaldoinicontrato, '999G999G999G990D99')) as coesaldoinicontrato FROM evento.contratopregao WHERE coeid = {$coeid};
        ";
        $coesaldoinicontrato = desformata_valor( $db->pegaUm($sql) );

        $sql = "
            SELECT sum(uresaldoinicontrato)::numeric(12,2) as total FROM evento.unidaderecurso WHERE coeid = {$coeid};
        ";
        $total_saldoinicontrato = $db->pegaUm($sql);

        if( $uresaldoinicontrato <= ( $coesaldoinicontrato - $total_saldoinicontrato ) ){
            if( $ureid == '' ){
                $sql = "
                    INSERT INTO evento.unidaderecurso(
                            preid, ungcod, coeid, ureordenador, ureordenadorsub, uresaldoinicontrato
                        )VALUES(
                            {$preid}, '{$ungcod}', {$coeid}, '{$ureordenador}', '{$ureordenadorsub}', '{$uresaldoinicontrato}'
                    ) RETURNING ureid;
                ";
            }else{
                $sql = "
                    UPDATE evento.unidaderecurso
                        SET preid               = {$preid},
                            ungcod              = {$ungcod},
                            coeid               = {$coeid},
                            ureordenador        = '{$ureordenador}',
                            ureordenadorsub     = '{$ureordenadorsub}',
                            uresaldoinicontrato = '{$uresaldoinicontrato}'
                    WHERE ureid= {$ureid} RETURNING ureid;
                ";
            }
            $ureid = $db->pegaUm($sql);
            $msg = "A operação foi realizada com sucesso!";
        }else{
             if( $ureid != '' ){
                $sql = "
                    UPDATE evento.unidaderecurso
                        SET ureordenador        = '{$ureordenador}',
                            ureordenadorsub     = '{$ureordenadorsub}'
                    WHERE ureid= {$ureid} RETURNING ureid;
                ";
                $ureid = $db->pegaUm($sql);
            }
            $msg = "Dados descritivos dessa Unidade de Recurso foram atualizados com sucesso.";
            $erro_saldo = 'S';
        }

        if( $ureid > 0){
            $db->commit();
            $db->sucesso( 'principal/eventos/cad_eve_und_recurso', '', $msg);
        }else{
            $msg = $erro_saldo == 'S' ? "O saldo desse Contrato não é sufuciente para custear essa Unidade de Recurso" : "Não foi possível realizar a operação, tente novamente mais tarde!";
            $db->rollback();
            $db->sucesso( 'principal/eventos/cad_eve_und_recurso', '', "{$msg}");
        }
    }

    /**
     * functionName salvaItensConsumidoEvento
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function salvaItensConsumidoEvento(){
	global $db;

        $eveid = $_SESSION['evento']['eveid'];

        $itcid              = $_REQUEST['itcid'];
        $itvid              = $_REQUEST['itvid'];
        $valoritem          = $_REQUEST['hid_valoritem'];
        $iteid              = $_REQUEST['tipoitemevento'][0];
        $qtd_total_itens    = $_REQUEST['qtd_total_itens'];
        $itcparceria        = $_REQUEST['itcparceria'] == 'on' ? 't' : 'f';
        $valor_total_itens  = desformata_valor( $_REQUEST['valor_total_itens'] );

        #BUSCA CONTRATODO EVENTO.
        $_sql = "SELECT coeid FROM evento.empenho_unidade WHERE emuid = {$_SESSION['evento']['emuid']}";
        $coeid = $db->pegaUm($_sql);

        #BUSCAR SALDO DO EMPENHO RELACIONADO AO EVENTO.
        $saldo_empenho = verificaSaldoEmpenho( $eveid );

        #BUSCA USADO EM EVENTO ATE O MOMENTO.
        $valor_usado_evento = verificaSaldoEvento( $eveid );

        #SOMA DO VALOR USADO ATE O MOMENTO COM OS VALOR TOTAL DOS ITENS A SEREM GRAVADOS.
        $valor_totalisado = ($valor_usado_evento + $valor_total_itens);

        #VERIFICA SE A SALDO NO EMPENHO PARA ESSE ITEM.
        if( $saldo_empenho >= $valor_totalisado ){

            $qtd_disponivel = quantidadeDisponivelItens( $iteid, $coeid );

            if( $qtd_total_itens <= $qtd_disponivel && $qtd_disponivel > 0 ){

                $_sql = "SELECT itvid FROM evento.itemconsumo WHERE eveid = {$eveid} AND itvid = {$itvid} GROUP BY itvid;";
                $itcid_existe = $db->pegaUm($_sql);

                $msg = " Cadastro o item para o evento foi ralizado com sucesso! ";

                #USADO PARA ATUALIZAR OS ITENS  UDADOS NO EVENTO.
                if( $itcid_existe > 0 ){
                    $sql = "DELETE FROM evento.itemconsumo WHERE eveid = {$eveid} AND itvid = {$itvid};";
                    $db->executar($sql);

                    $msg = " Itens já cadastrados para o evento e sua quantidade foi atualizada com sucesso! ";
                }

                foreach( $_REQUEST['qtd'] as $key => $qtd ){
                    $qtd = $qtd != '' ? $qtd : 0;

                    $sql = "
                        INSERT INTO evento.itemconsumo(
                                itvid, eveid, itcquantidade, itcvalor, itcdia, itcdatainclusao, itcparceria
                            )VALUES(
                                {$itvid}, {$eveid}, '{$qtd}', '{$valoritem}', '{$key}', 'NOW()', '{$itcparceria}'
                        ) RETURNING itcid;
                    ";
                    $in_itcid = $db->pegaUm($sql);
                }

                if( $in_itcid > 0 ){
                    #BUSCA USADO EM EVENTO ATE O MOMENTO.
                    $valor_evento = verificaSaldoEvento( $eveid );
                    $sql = " UPDATE evento.evento SET evecustoprevisto = '{$valor_evento}' WHERE eveid = {$eveid}; ";
                    $db->executar($sql);
                }
                $erro = 'N';

            }else{
                if( $qtd_disponivel == 0 ){
                    $msg = "A quantidade de Itens disponível é (zero) 0, o iten não pode ser usado!";
                    $erro = 'S';
                }else{
                    $msg = "A quantidade Itens disponível é insuficiente, reveja a quantidade de itens a ser usado!";
                    $erro = 'S';
                }
            }
        }else{
            $msg = "O saldo disponível é insuficiente para custear esse item, verifique a saldo e tente novamente!";
            $erro = 'S';
        }

        if( $in_itcid > 0){
            $db->commit();
            $db->sucesso( 'principal/eventos/cad_eve_infra', '', "{$msg}");
        }else{
            $msg = $erro == 'S' ? $msg : "Não foi possível realizar a operação, tente novamente mais tarde!";
            $db->rollback();
            $db->sucesso( 'principal/eventos/cad_eve_infra', '', "{$msg}");
        }
    }

    /**
     * functionName verificaSaldoEmpenho
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function verificaSaldoEmpenho( $eveid ){
        global $db;

        #BUSCAR SALDO DO EMPENHO RELACIONADO AO EVENTO.
        $sql = "
            SELECT  trim( to_char( empsaldoinicontrato, '999G999G999G990D99' ) ) AS empsaldoinicontrato
            FROM evento.evento AS e
            JOIN evento.empenho_unidade AS eu On eu.emuid = e.emuid
            WHERE e.eveid = {$eveid}
        ";
        return $saldo_empenho = desformata_valor( $db->pegaUm($sql) );
    }

    /**
     * functionName verificaSaldoEvento
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function verificaSaldoEvento( $eveid ){
        global $db;

        #BUSCA USADO EM EVENTO ATE O MOMENTO.
        $sql = "
            SELECT trim( to_char( SUM( (itcvalor * itcquantidade) ), '999G999G999G990D99' ) ) AS valor_usado_evento
            FROM evento.itemconsumo AS i
            WHERE i.eveid = {$eveid}
        ";
        return $valor_usado_evento = desformata_valor( $db->pegaUm($sql) );
    }

    /**
     * functionName verificaSaldoContrato
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function verificaSaldoContrato( $dados ){
        global $db;

        #SS - SEM SALDO.
        #TS - TEM SALDO.

        $coeid = trim( $dados['coeid'] );
        $valor = desformata_valor( $dados['valor'] );

        $sql = "
            SELECT trim(to_char(coesaldoinicontrato, '999G999G999G990D99')) as coesaldoinicontrato FROM evento.contratopregao WHERE coeid = {$coeid};
        ";
        $coesaldoinicontrato = desformata_valor( $db->pegaUm($sql) );

        $sql = "
            SELECT trim(to_char(sum(uresaldoinicontrato), '999G999G999G990D99')) as total FROM evento.unidaderecurso WHERE coeid = {$coeid};
        ";
        $total_saldoinicontrato = desformata_valor( $db->pegaUm($sql) );

        if( $valor > ( $coesaldoinicontrato - $total_saldoinicontrato ) ){
            $dispon = formata_valor( $coesaldoinicontrato - $total_saldoinicontrato );
            $data['result'] = 'SS';
            $data['msg'] = iconv("ISO-8859-1", "UTF-8", "Não a saldo disponivel para esse contrato. O valor disponivel é: R$ {$dispon}" );
        }else{
            $data['result'] = 'TS';
        }
        echo simec_json_encode( $data );
        die;
    }

    /**
     * functionName verificaSaldoUnidade
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados REQUEST do formulario
     * @return string persistencia dos dados
     *
     * @version v1
    */
    function verificaSaldoUnidade( $dados ){
        global $db;

        #SS - SEM SALDO.
        #TS - TEM SALDO.

        $ureid = trim( $dados['ureid'] );
        $valor = desformata_valor( $dados['valor'] );

        #BUSCA SALDO E VALOR TOTAL.
        $sql = "
            SELECT trim(to_char(uresaldoinicontrato, '999G999G999G990D99')) as uresaldoinicontrato FROM evento.unidaderecurso WHERE ureid = {$ureid};
        ";
        $uresaldoinicontrato = desformata_valor( $db->pegaUm($sql) );

        $sql = "
            SELECT trim(to_char(sum(empsaldoinicontrato), '999G999G999G990D99')) AS total_empenho FROM evento.empenho_unidade WHERE ureid = {$ureid};
        ";
        $total_empenho = desformata_valor( $db->pegaUm($sql) );

        if( $valor > ( $uresaldoinicontrato - $total_empenho ) ){//verificar a formatação.
            $dispon = formata_valor( $uresaldoinicontrato - $total_empenho );
            $data['result'] = 'SS';
            $data['msg'] = iconv("ISO-8859-1", "UTF-8", "Não a saldo disponivel para essa unidade. O valor disponivel é: R$ {$dispon}" );
        }else{
            $data['result'] = 'TS';
        }
        echo simec_json_encode( $data );
        die;
    }

    /**
     * functionName pegaEstadoAtualEvento
     *
     * @author Luciano F. Ribeiro
     * @info PEGA ESDATO ATUAL DO EVENTO.
     *
     * @param integer variavel de sessao, id do evento.
     *
     * @return integer estado atual do evento.
     *
     * @version v1
    */
    function pegaEstadoAtualEvento(){
        global $db;

        $eveid = $_SESSION['evento']['eveid'];

        if( $eveid ){
            $sql = "
                SELECT  d.esdid
                FROM evento.evento e
                JOIN workflow.documento as d on d.docid = e.docid
                WHERE e.eveid = {$eveid};
            ";
            $esdid = $db->pegaUm($sql);
        }
        return $esdid;
    }


    /**
     * functionName quantidadeDisponivelItens
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer $item id do item a verificar a quantidade.
     * @param integer $coeid id do contrato relacionado ai item.
     *
     * @return integer quantidade do item.
     *
     * @version v1
    */
    function quantidadeDisponivelItens( $item, $coeid ){
        global $db;

        #BUSCA QUANTIDADE MAXIMA DE ITENS
        $sql = "
            SELECT  iqtqtdmax AS qtd_max
            FROM evento.itemquantidade AS i
            WHERE i.iteid = {$item} AND i.coeid = {$coeid};
        ";
        $qtd_max = $db->pegaUm($sql);

        #BUSCAS QUANTOIDADE DE ITENS USDADOS.
        $sql = "
            SELECT SUM(itcquantidade) AS qtd_usada
            FROM evento.itemconsumo AS ic
            JOIN evento.itemvalor AS iv ON iv.itvid = ic.itvid
            WHERE iv.iteid = {$item} AND iv.coeid = {$coeid};
        ";
        $qtd_usada = $db->pegaUm($sql);

        return $qtd_disponivel = ($qtd_max - $qtd_usada);
    }

?>