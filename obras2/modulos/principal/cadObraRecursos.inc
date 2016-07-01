<?php
// empreendimento || obra || orgao
verificaSessao( 'obra' );

//if($_GET['download']){
//    $arqid = $_GET['arqid'];
//    if ($arqid) {
//        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
//        $file = new FilesSimec();
//        $file->getDownloadArquivo($arqid);
//    }
//}


function salvar(){
	
	global $db;
	
	if( $_REQUEST['frpid'] != '' ){
		
		$obras = new Obras( $_SESSION['obras2']['obrid'] );
		$obras->frpid = $_REQUEST['frpid'];
		$obras->stiid = $_REQUEST['stiid'];
		$obras->medidasexcecao = ($_REQUEST['medidasexcecao'] == 'S') ? true : false;

	    $arCamposNulo = array();
		if ( $_REQUEST['tooid'] ){
			$obras->tooid = $_REQUEST['tooid'];
		}else{
			$obras->tooid = null;
			$arCamposNulo[] = 'tooid';	
		}	
		
		$obras->salvar(true, true, $arCamposNulo);
		$obras->commit();
?>
	<script>
		alert('Operação realizada com sucesso!');
		window.location.href = window.location.href;
	</script>
<?php 
	}
}

function exibeObrasConvenio(){
	global $db;
	
	include_once(APPRAIZ . "/obras2/modulos/principal/exibeObrasConvenio.inc");
	die;
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];

if( $_GET['acao'] != 'V' ){
    //Chamada de programa
    include  APPRAIZ."includes/cabecalho.inc";
    echo "<br>";
    if( !$_SESSION['obras2']['obrid'] && !$_SESSION['obras2']['empid'] ){
            $db->cria_aba(ID_ABA_CADASTRA_OBRA_EMP,$url,$parametros);
    }elseif( $_SESSION['obras2']['obrid'] ){
            if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
                    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
            }else{
                    $db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
            }
    }else{
            $db->cria_aba(ID_ABA_CADASTRA_OBRA,$url,$parametros);
    }
	
}else{
    echo'<script language="JavaScript" src="../includes/funcoes.js"></script>
         <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
         <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

    $db->cria_aba($abacod_tela,$url,$parametros);

}

$obrasArquivos 	    = new ObrasArquivos();
$obras              = new Obras( $_SESSION['obras2']['obrid'] );
$frpid              = $obras->frpid;
$stiid              = $obras->stiid;
$medidasexcecao     = $obras->medidasexcecao;
$tooid 	            = $obras->tooid;
$numconvenio        = $obras->numconvenio;
$obrnumprocessoconv = $obras->obrnumprocessoconv;
$obranoconvenio	    = $obras->obranoconvenio;
$orgid              = $_SESSION['obras2']['orgid'];

if( possui_perfil( array(PFLCOD_SUPER_USUARIO)) ){
    $habilitado = true;
    $habilita = 'S';
}else{
    $habilitado = false;
    $habilita = 'N';
}

echo cabecalhoObra($obrid);
echo "<br>";
monta_titulo( 'Recursos da Obra', '' );

$tipoOrigemObra = new TipoOrigemObra();

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
    $(function () {
        $('#tooid').change(function () {
            if($(this).val() == '2'){
                $('#convenio').show();
            } else {
                $('#convenio').hide();
            }
        });
    });

function abreCamposRecursos(id){
	var tr_convenio  	= document.getElementById( 'convenio' );
	var tr_descentralizacao = document.getElementById( 'descentralizacao' );
	var tr_rec  	        = document.getElementById( 'recurso' );
	
	if(tr_descentralizacao && tr_rec) {
		if(id == ''){
			if (document.selection){
				tr_descentralizacao.style.display 	= 'none';
				tr_rec.style.display 			= 'none';
			}else{
				tr_descentralizacao.style.display 	= 'none';
				tr_rec.style.display 			= 'none';
			}
		}
		
		if(id == <?php echo FRPID_DESCENTRALIZACAO ?>){
			if (document.selection){
				tr_descentralizacao.style.display = 'block';
				tr_rec.style.display 		  = 'none';
			}else{
				tr_descentralizacao.style.display = 'table-row';
				tr_rec.style.display 		  = 'none';
			}
		}
		
		if(id == <?php echo FRPID_RECURSO_PROPRIO ?>){
			if (document.selection){
				tr_descentralizacao.style.display = 'none';
				tr_rec.style.display 		  = 'block';
			}else{
				tr_descentralizacao.style.display = 'none';
				tr_rec.style.display 		  = 'table-row';
			}
		}
	}
}

$(document).ready(function(){
	
	abreCamposRecursos(<?php echo $frpid ?>);
	
	$('[type="text"]').keyup();
	
	$('#salvar').click(function(){
		var stop = false;
		$('.obrigatorio').each(function(){
			if( $(this).val() == '' ){
				stop = true;
				alert('Preencha todos os campos obrigatórios.');
				$(this).focus();
				return false;
			}
		});
		if( stop ){
			return false;
		}
		$('#req').val('salvar');
		$('#formRecursos').submit();
	});
	
});
</script>
<form method="post" name="formRecursos" id="formRecursos">
	<input type="hidden" name="req" id="req" value=""/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
                    <td style="width:20%" class="SubTituloDireita">Tipo de Instrumento</td>
                    <td>
                        <?php 
                            $tipoFormaRepasseRecursos = new TipoFormaRepasseRecursos();
                            $sql                      = $tipoFormaRepasseRecursos->listaCombo($frpid);
                            $db->monta_combo('frpid', $sql, $habilita, "Selecione...", 'abreCamposRecursos', '', '', '150', 'S', 'frpid');
                        ?>	
                    </td>
		</tr>
        <tr>
            <td style="width:20%" class="SubTituloDireita">Situação do Instrumento</td>
            <td>
                <?php
                $tipoSituacaoInstrumento = new SituacaoInstrumento();
                $sql                      = $tipoSituacaoInstrumento->listaCombo();
                $db->monta_combo('stiid', $sql, $habilita, "Selecione...", '', '', '', '150', 'S', 'stiid');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Medidas de Exceção</td>
            <td>
                <input type="radio" name="medidasexcecao" <?=($habilita == 'N') ? "disabled" : ""?> id="" value="S" <?= ( $medidasexcecao == 't' ? "checked='checked'" : "" ) ?>/> Sim
                <input type="radio" name="medidasexcecao" <?=($habilita == 'N') ? "disabled" : ""?> id="" value="N" <?= ( $medidasexcecao == 'f' ? "checked='checked'" : "" ) ?>/> Não
            </td>
        </tr>
		<tr>
                    <td class="SubTituloDireita">Fonte:</td>
                    <td>
                        <?php 
                            $sql = $tipoOrigemObra->listaComboRecursos($obras->preid);
                            $db->monta_combo('tooid',$sql, 'N','Selecione...','','','',200,'N', 'tooid', '', '', '');
                        ?>
                    </td>
		</tr>
		<tr id="convenio" style="<?=($tooid == 2 ? '' : 'display: none;')?>">
			<td colspan="2">
                <?php
                $sql = "SELECT
                                                                        b.*,
                                                                        CASE WHEN b.dcoid IS NULL THEN a.obrnumprocessoconv ELSE b.dcoprocesso END as dcoprocesso,
                                                                        CASE WHEN b.dcoid IS NULL THEN a.obranoconvenio ELSE b.dcoano END as dcoano,
                                                                        CASE WHEN b.dcoid IS NULL THEN a.numconvenio ELSE b.dcoconvenio END as dcoconvenio
                                                                FROM
                                                                        obras2.obras a
                                                                LEFT JOIN painel.dadosconvenios b on b.dcoprocesso = Replace(Replace(Replace(a.obrnumprocessoconv,'.',''),'/',''),'-','')
                                                                WHERE
                                                                        obrid = ".$_SESSION["obras2"]["obrid"];

                $rsConvenio = $db->pegaLinha($sql);
                ?>

				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
					<tr>
						<td>Convênio
                            <? if (!$rsConvenio['dcoid']): ?>
                                <span style="color: red">(dados bancários não encontrados)</span>
                            <? endif; ?>
                        </td>
					</tr>
					<tr>
                                            <td class="SubTituloDireita">Número do Convênio</td>
                                            <td>
                                                <?
                                                    $covnumero = $rsConvenio['dcoconvenio']; //$dados_convenio["covnumero"];
                                                    echo campo_texto( 'covnumero', 'N', 'N', '', 22, 20, '', '', 'left', '', 0, 'id="covnumero"');
                                                    if($habilitado == 'N'){
                                                        echo '<!--
                                                              <input type="button" name="busca_convenio" value="Pesquisar" style="cursor: pointer;" onclick="window.open( \'?modulo=principal/inserir_convenio&acao=A\', \'blank\', \'height=600, width=800, status=no, toolbar=no, menubar=no, scrollbars=yes, location=no, resizable=no\');"/>
                                                              <input type="button" name="limpa_convenio" value="Remover" style="cursor: pointer;" onclick="removeConvenio();"/>
                                                              -->';
                                                    }
                                                ?>
                                                    <input type="hidden" name="covid" id="covid" value="<?php if($covid){ echo $covid; } ?>"/>

                                                <?php 
                                                    $sql = "SELECT 
                                                                    count(*) 
                                                            FROM 
                                                                    obras2.obras a 
                                                            WHERE 
                                                                    a.numconvenio = '{$numconvenio}'
                                                                AND a.obranoconvenio = '{$obranoconvenio}'";
                                                    $rsObrasComMesmoConvenio = $db->pegaUm($sql);							
                                                    if($rsObrasComMesmoConvenio > 1): 
                                                ?>
                                                        <script type="text/javascript">
                                                            function popupConvenioObras(covnumero){
                                                                var popUp = window.open('?modulo=principal/cadObraRecursos&acao=A&req=exibeObrasConvenio&covnumero='+covnumero, 'popupConvenioObras', 'height=600,width=800,scrollbars=yes,top=0,left=0');
                                                                popUp.focus();
                                                            }
                                                        </script>
                                                        <a href="javascript:void(0)" onclick="popupConvenioObras('<?=$numconvenio; ?>')">
                                                                &nbsp;Obras com o mesmo convênio
                                                        </a>
                                                <?php endif; ?>
                                            </td>
					</tr>
					<tr>
                                            <td class="SubTituloDireita">Ano</td>
                                            <td>
                                                <?php 
                                                    $covano = $rsConvenio['dcoano']; //$dados_convenio["covano"];
                                                    echo campo_texto( 'covano', 'N', 'N', '', 4, 5, '', '', 'left', '', 0, 'id="covano"'); 
                                                ?>
                                            </td>
					</tr>
					<tr>
                                            <td class="SubTituloDireita">Processo</td>
                                            <td>
                                                <?php 
                                                    $covprocesso = $rsConvenio['dcoprocesso']; //$dados_convenio["covprocesso"]; 
                                                    echo campo_texto( 'covprocesso', 'N', 'N', '', 22, 20, '', '', 'left', '', 0, 'id="covprocesso"'); 
                                                ?>
                                            </td>
					</tr>
					<tr>
                                            <td class="SubTituloDireita">Concedente</td>
                                            <td>
                                                <?php 
                                                    $covvlrconcedente = $rsConvenio['dcovalorconcedente']; //$dados_convenio["covvlrconcedente"];
                                                    $covvlrconcedente = number_format($covvlrconcedente,2,',','.'); 
                                                    echo campo_texto( 'covvlrconcedente', 'N', 'N', '', 22, 17, '###.###.###,##', '', 'left', '', 0, 'id="covvlrconcedente"'); 
                                                ?>
                                            </td>
					</tr>
					<tr>
                                            <td class="SubTituloDireita">Convenente</td>
                                            <td>
                                                <?php 
                                                    $covvlrconvenente = $rsConvenio['dcovalorcontrapartida']; //$dados_convenio["covvlrconvenente"];
                                                    $covvlrconvenente = number_format($covvlrconvenente,2,',','.'); 
                                                    echo campo_texto( 'covvlrconvenente', 'N', 'N', '', 22, 17, '###.###.###,##', '', 'left', '', 0, 'id="covvlrconvenente"'); 
                                                ?>
                                            </td>
					</tr>
					<tr>
                                            <td class="SubTituloDireita">Valor R$</td>
                                            <td>
                                                <?php 
                                                    $covvalor = $rsConvenio['dcovalorcontrapartida']+$rsConvenio['dcovalorconcedente']; //$dados_convenio["covvalor"];
                                                    $covvalor = number_format($covvalor,2,',','.'); 
                                                    echo campo_texto( 'covvalor', 'N', 'N', '', 22, 17, '###.###.###,##',  '', 'left', '', 0, 'id="covvalor"'); 
                                                ?>
                                            </td>
					</tr>
					<tr>
                                            <td class="SubTituloDireita">Início</td>
                                            <td>
                                                <?php 
                                                    $covdtinicio = $rsConvenio['dcodatainicio']; //$dados_convenio["covdtinicio"]; 
                                                    echo campo_data2( 'covdtinicio', 'N', 'N', '', 'S' ); 
                                                ?>
                                            </td>
					</tr>
					<tr>
                                            <td class="SubTituloDireita">Fim</td>
                                            <td>
                                                <?php 
                                                    $covdtfinal = $rsConvenio['dcodatafim']; //$dados_convenio["covdtfinal"]; 
                                                    echo campo_data( 'covdtfinal', 'N', 'N', '', 'S' ); 
                                                ?>
                                            </td>
					</tr>
					<tr>
						<td colspan="2">Saldos Bancários</td>
					</tr>
					<tr>
                                            <td colspan="2">
                                                <div id="covaditivo">
                                                <?php
                                                    $dados_aditivo = "SELECT 
                                                                            substring(dfidatasaldo::varchar,6,2) || '/' || 
                                                                            substring(dfidatasaldo::varchar,0,5), SUM(dfisaldoconta) as dfisaldoconta, 
                                                                            (SUM(dfisaldofundo) + SUM(dfisaldopoupanca) + SUM(dfisaldordbcdb)) AS totalaplicacao, 
                                                                            (SUM(dfisaldoconta) + SUM(dfisaldofundo) + SUM(dfisaldopoupanca) + SUM(dfisaldordbcdb)) AS totalconta
                                                                      FROM 
                                                                            painel.dadosfinanceirosconvenios dfi
                                                                      WHERE 
                                                                            dfiprocesso = '" . $rsConvenio['dcoprocesso'] . "'
                                                                      GROUP BY 
                                                                            dfidatasaldo
                                                                      ORDER BY 
                                                                            dfidatasaldo";
                                                    $cabecalho = array('Mês Ref.','Saldo Conta','Saldo Aplicações','Total Conta');
                                                    $db->monta_lista_simples( $dados_aditivo, $cabecalho, 1000, 10, 'N', '90%', '' );
                                                 ?>
                                                </div>
                                            </td>
					</tr>
					<tr>
                                            <td colspan="2">Repasses</td>
					</tr>
					<tr>
                                            <td colspan="2">
                                                <div id="covaditivo">
                                                <?php
                                                    $dados_aditivo = "SELECT 
                                                                            to_char(drcdatapagamento, 'DD/MM/YYYY') as datapagamento, 
                                                                            drcordembancaria, 
                                                                            drcvalorpago, 
                                                                            'Banco: ' || drcbanco || '<br>Ag: ' || drcagencia || '<br>CC: ' || drcconta as drcdadosbancarios
                                                                      FROM 
                                                                           painel.dadosrepassesconvenios drc
                                                                      WHERE 
                                                                           drcprocesso = '" . $rsConvenio['dcoprocesso'] . "'
                                                                     ORDER BY 
                                                                            drcdatapagamento";
                                                    $cabecalho = array('Data do Repasse','OB','Valor Repassado','Dados Bancários');
                                                    $db->monta_lista_simples( $dados_aditivo, $cabecalho, 1000, 10, 'N', '90%', '' );
                                                 ?>
                                                </div>
                                            </td>
					</tr>

				</table>
			</td>
		</tr>
<?php
if($obras->preid) :
	$preid = $obras->preid;
?>
        <?php if ($tooid == 1 || $tooid == 11 || $tooid == 4): ?>
        <tr>
            <th colspan="2">Pagamentos</th>
        </tr>

        <tr>
            <td colspan="2">
                <?php $dadosPagemento = $obras->getPagamentoPar($obras->obrid); ?>
                <?php if($dadosPagemento): ?>

                    <?php
                    $valorTotalPago = 0;
                    $percentTotalPago = 0;
                    foreach ($dadosPagemento as $key => $pag){
                        unset($dadosPagemento[$key]['pagnumeroob']);
                        $percentTotalPago = array_pop($dadosPagemento[$key]);
                        $valorTotalPago = array_pop($dadosPagemento[$key]);
                    }
                    $cabecalho = array('Nº do Termo','Número processo','Número do Empenho','Valor do Pagamento','Percentual pagamento','Parcela','Data pagamento','Situação', 'Valor da Obra');
                    $db->monta_lista_simples($dadosPagemento, $cabecalho, 50, 5, array('4'=>'4','3'=>'3'), '100%', 'S', false, false, false, false);
                    ?>
                <?php else: ?>
                    Nenhum pagamento efetuado.
                <?php endif; ?>
            </td>
        </tr>
        <?php endif; ?>
		<tr>
			<th colspan="2">Financeiro</th>
		</tr>
		<tr>
			<td colspan="2">
			<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="SubTituloDireita" valign="top"><b>Termo:</b></td>
					<td valign="top">
                                            <?php
                                                /* Trecho que preenche a coluna termo */
                                                $sql = "SELECT terassinado, 
                                                               to_char(terdatainclusao,'dd/mm/YYYY') as terdatainclusao, 
                                                               tcp.arqid FROM par.termoobra teo 
                                                                INNER JOIN par.termocompromissopac tcp ON teo.terid = tcp.terid 
                                                                WHERE teo.preid = '".$preid."' AND terstatus = 'A'

                                                         UNION

                                                            SELECT
                                                                CASE WHEN dv.dpvid IS NOT NULL THEN TRUE ELSE FALSE END terassinado,
                                                                to_char(dp.dopdatainclusao,'dd/mm/YYYY') as terdatainclusao,
                                                                dp.arqid_documento as arqid
                                                            FROM
                                                                par.termocomposicao tc
                                                            INNER JOIN par.documentopar  dp ON dp.dopid = tc.dopid
                                                            LEFT JOIN par.documentoparvalidacao dv ON dv.dopid = dp.dopid
                                                            WHERE tc.preid = '".$preid."' AND dp.dopstatus = 'A'
                                                                ";

                                                $termoobra = $db->pegaLinha($sql);

                                                if($termoobra) {
                                                        if($termoobra['terassinado']=="f") {
                                                                echo "Gerado (".($termoobra['terdatainclusao']).")";	
                                                        } elseif($termoobra['terassinado']=="t") {
                                                                echo "Assinado";
                                                        }
                                                        if($termoobra['arqid']) {
                                                                echo " <a href=\"/par/par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&download=S&arqid=".$termoobra['arqid']."\" ><img align=absmiddle src=../imagens/anexo.gif border=0></a>";
                                                        }
                                                } else {
                                                        echo "Não gerado";	
                                                }
                                                /* FIM - Trecho que preenche a coluna termo */
                                            ?>
					</td>
					<td class="SubTituloDireita" valign="top"><b>Empenho:</b></td>
					<td valign="top">
                                            <?php
                                                /* Trecho que preenche a coluna empenho */
                                                $sql = "
                                                    SELECT
                                                    ne as empnumero,
                                                    valorempenho as empvalorempenho
                                                    FROM par.v_saldo_obra_por_empenho
                                                    WHERE preid = '".$preid."' AND valorempenho > 0";
                                                $empenhoobra = $db->pegaLinha($sql);

                                                if($empenhoobra) {
                                                        if($empenhoobra['empnumero']) {
                                                                echo "Gerado (R$".number_format($empenhoobra['empvalorempenho'],2,",",".")." - ".$empenhoobra['empnumero'].")";
                                                        } else {
                                                                echo "Não gerado";
                                                        }
                                                } else {
                                                        echo "Não gerado";	
                                                }
                                                /* FIM - Trecho que preenche a coluna empenho */
                                            ?>
					</td>
					<td class="SubTituloDireita" valign="top"><b>Pagamento:</b></td>
					<td valign="top">
                                            <?php

                                            /* Substituido dia 23/12/2013 parnumseqob ->   pagnumeroob
                                             * Analista: Daniel Areas Programador Eduardo Duniceu
                                            * */
                                            /* Trecho que preenche a coluna pagamento */
                                            $sql = "SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagsolicdatapagamento, to_char(pag.pagdatapagamentosiafi ,'dd/mm/YYYY') as pagdatapagamento
                                                    FROM par.v_saldo_obra_por_empenho emp
                                                    INNER JOIN (

                                                            SELECT pronumeroprocesso, probanco, proagencia FROM par.processoobra pro WHERE pro.prostatus = 'A'
                                                            UNION
                                                            SELECT pronumeroprocesso, probanco, proagencia FROM par.processoobraspar pro WHERE pro.prostatus = 'A'

                                                    ) pro ON pro.pronumeroprocesso = emp.processo
                                                    INNER JOIN par.pagamento pag ON pag.empid = emp.empid
                                                    WHERE emp.preid = '".$preid."' AND pag.pagstatus='A' AND emp.valorempenho > 0
                                                    ORDER BY emp.empid, pag.pagdatapagamento ASC";
                                            $pagamentoobra = $db->pegaLinha($sql);

                                            if($pagamentoobra) {
                                                    echo "Pago<br>
                                                              Valor pagamento(R$): ".number_format($pagamentoobra['pagvalorparcela'],2,",",".")."<br>
                                                              Nº da Ordem Bancária: ".$pagamentoobra['pagnumeroob']."<br>
                                                              Data de Solicitação do pagamento: ".$pagamentoobra['pagsolicdatapagamento']."<br>
                                                              Data do pagamento: ".$pagamentoobra['pagdatapagamento']."<br>
                                                              Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia']."
                                                              ";
                                            } else {
                                                    echo "Não pago";	
                                            }
                                            /* FIM - Trecho que preenche a coluna pagamento */
                                            ?>
					</td>
				</tr>
			</table>
			</td>
		</tr>
                <?php
                endif;
		if( $empenhoobra['empnumero'] ){
			$processo = $db->pegaUm("select e.empnumeroprocesso from par.empenho e where e.empnumero = '{$empenhoobra['empnumero']}' and empstatus = 'A'");
			if( $processo ){?>		
				<tr>
					<!--<th colspan="2">Saldos</th>-->
					<th colspan="2">Extrato</th>
				</tr>
				<tr>
                    <td colspan="2">

                        <script type="text/javascript" src="../../includes/remedial.js"></script>
                        <script type="text/javascript" src="../../includes/superTitle.js"></script>
                        <link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>

                        <?php
//                            $sql = "select '<span class=\'processo_detalhe\' >'||po.ppaprocesso||'</span>' as ppaprocesso, 
//                                            po.ppacnpj, 
//                                            po.pparazaosocial, 
//                                            po.ppabanco, 
//                                            po.ppaagencia, 
//                                            po.ppaconta,
//                                            po.ppasaldoconta, 
//                                            po.ppasaldofundos, 
//                                            po.ppasaldopoupanca, 
//                                            po.ppasaldordbcdb
//                                    from obras2.pagamentopac po
//                                    where
//                                        po.ppaprocesso = '$processo'";
                            $sql = "SELECT 
                                            '<span class=\"processo_detalhe\" >'||dfi.dfiprocesso||'</span>' as dfiprocesso,
                                            dfi.dficnpj,
                                            iue.iuenome as razao_social,
                                            dfi.dfibanco,
                                            dfi.dfiagencia,
                                            dfi.dficonta,
                                            dfi.dfisaldoconta,
                                            dfi.dfisaldofundo,
                                            dfi.dfisaldopoupanca,
                                            dfi.dfisaldordbcdb,
                                            dfi.dfimesanosaldo
                                    FROM painel.dadosfinanceirosconvenios dfi
                                    left join par.instrumentounidadeentidade iue on iue.iuecnpj = dfi.dficnpj
                                    WHERE dfiprocesso = '{$processo}'
                                    ORDER BY dfi.dfidatasaldo DESC;";
                            
                                  
                            $cabecalho = array('Processo', 'CNPJ', 'Razão Social', 'Banco', 'Agência', 'Conta', 'Saldo Conta', 'Saldo Fundos', 'Saldo Poupança', 'Saldo CDB','Mês/Ano Referência');
                            $db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%', 'S', false, false, false, false);
                            ?>
                    </td>
                </tr>
		<?php	}
		} ?>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<div style="float: left;">
					<!-- 
					<input type="hidden" name="frrid" value="<?=$frpid?>">
					-->
					<input type="hidden" name="obrid" value="<?=$_SESSION["obras2"]["obrid"];?>">
					<input type="button" id="salvar" value="Salvar" style="cursor: pointer" <?php if( $habilita == "N") { echo "disabled"; } ?>>
					<input type="button" value="Voltar" id="btnVoltar" style="cursor: pointer" onclick="history.back(-1);">
				</div>
			</td>
		</tr>
	</table>
</form>

<?php
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formRecursos'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#btnVoltar').attr('disabled', false);
                   }
                 ";
            echo '</script>';
        }
?>
