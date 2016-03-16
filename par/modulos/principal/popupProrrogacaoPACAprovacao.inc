<?php
if ($_POST['calculaDiasPAC'] == true) {
	
    global $db;
    
    $dataAtual 	= $_REQUEST['dataAtual'];
    $data 		= formata_data_sql($_REQUEST['data']);

    if ( $dataAtual >= $data) {
        echo 'erro';
        die();
    }

    if ( !validaData($dataAtual) || !validaData($data) ) {
        echo 'invalido';
        die();
    }

    $sql = "SELECT date '$data' - date '$dataAtual' as dias";
    $dias = $db->pegaUm($sql);

    echo $dias;
    die();
}

if ($_POST['requisicao'] == 'salva') {
    global $db;
    
    #Trata informacoes do post
    $data = $_POST['popprazoFinalProrrogacao'] ? formata_data_sql($_POST['popprazoFinalProrrogacao']) : formata_data_sql($_POST['popdataprazo']);
    $dias = $_POST['dias'] ? $_POST['dias'] : $_POST['qtddiassolichd'];
    
    if( !$_POST['popparecer'] || !$data ||!is_numeric($dias) || !$_POST['preid'] ){
    	echo '<script>alert("Dados inválidos.");
			        		window.opener.location.reload();
			        		window.close();</script>';
    	die();
    }
    
    #Atualiza informacoes no banco
    $sql = "UPDATE obras.preobraprorrogacao SET popstatus = 'I' WHERE popstatus = 'A' AND preid = {$_POST['preid']};
    		UPDATE obras.preobraprorrogacao 
    		SET popvalidacao = 't', popparecer = '".$_POST['popparecer']."', popdataprazoaprovado = '".$data."', popqtddiasaprovado = ".$dias.", popstatus = 'A', popdatavalidacao = 'NOW()' 
    		WHERE popstatus = 'P' AND preid = ".$_POST['preid']."; ";
    $db->executar($sql);
    
    $retorno = gerarDocumentoProrrogacao($_POST['preid'], $_POST['popparecer'], $data, $dias);
    if ($retorno == 'sucesso') {

        # Caso a obra seja aprovada o sistema faz update da tabela de documentopar para informar que existe um documento quee precisa ser regerado a vigencia
        $sql = "UPDATE par.documentopar SET dopprorrogacaovigenciaobra = 'true' WHERE dopid = (SELECT distinct 
        			dp.dopid
				FROM obras.preobra po
                INNER JOIN par.termocomposicao tc on tc.preid = po.preid
                INNER JOIN par.documentopar dp on dp.dopid = tc.dopid
                WHERE po.preid = {$_POST['preid']} AND dp.dopstatus='A')";
        $db->executar($sql);
        $db->commit();

        # Busca tipo de Obra
        $sql = "SELECT tooid FROM obras.preobra WHERE preid = {$_POST['preid']}";
        $tooid = $db->pegaUm($sql);

        # Se a obra for PAC altera estado.
        if ($tooid == 1) {

            include_once APPRAIZ . 'includes/workflow.php';
            $docid = $db->pegaUm("SELECT docid FROM obras.preobra WHERE preid='" . $_POST['preid'] . "'");

            $esdidorigem = WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO;

            $esdiddestino = $db->pegaUm("SELECT esdidorigem FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid=" . $_POST['preid']);
            $esdiddestino = $esdiddestino != '' ? $esdiddestino : WF_TIPO_OBRA_APROVADA;

            $aedid = $db->pegaUm("SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem = " . $esdidorigem . " AND esdiddestino = " . $esdiddestino);
		
			if( empty($aedid) ){
				$sql = "SELECT esddsc FROM workflow.estadodocumento WHERE esdid = $esdiddestino LIMIT 1";
				$esddsc = $db->pegaLinha( $sql );
					
				$sql = "INSERT INTO workflow.acaoestadodoc(esdidorigem, esdiddestino, aeddscrealizar, aeddscrealizada, aedcondicao, aedobs, aedposacao, aedvisivel, aedicone, aedcodicaonegativa)
						VALUES ($esdidorigem, $esdiddestino, 'Retornar para $esddsc', 'Retornar para $esddsc', '', '', '', false, '', false) RETURNING aedid";
					
				$aedid = $db->pegaUm( $sql );
			}

            $result = wf_alterarEstado($docid, $aedid, true, array('preid' => $_POST['preid']));
        }
        
        if( $result || $tooid != 1 ){
        	
	        if( $_REQUEST['acompanhamento'] == 'S' ){
	        	echo "<script>alert('O documento foi anexado a lista de obras.');
		        		window.opener.carregarListaDivObras('".$_REQUEST['processo']."', '');
		        		window.close();</script>";
	        } else {
		        echo '<script>alert("O documento foi anexado a lista de obras.");
			        		window.opener.location.reload(); 
			        		window.close();</script>';
	        }
        }else{
        	
	        echo '<script>alert("Erro ao tramitar.");
		        		window.opener.location.reload(); 
		        		window.close();</script>';
			$db->rollback();
        }
    } else {
    	if( $_REQUEST['acompanhamento'] == 'S' ){
			echo "<script>alert('Erro.');
	        		window.opener.carregarListaDivObras('".$_REQUEST['processo']."', '');
	        		window.close();</script>";
		} else {
			echo '<script>alert("Erro.");
						window.opener.location.reload();
					window.close();</script>';
		}
		$db->rollback();
    }
    die();
}

#Recusar Prazo
if ($_POST['requisicao'] == 'recusar') {
    global $db;
    if($_REQUEST['boSolicitacao'] == 'S'){
	    $sql = "update obras.preobra set preblockreformulacao = 'S' where preid = {$_REQUEST['preid']}";
	    $db->executar($sql);    
	    $db->commit();
    }
   
    recusarProrrogacaoObrasPAC($_REQUEST);
    
    die();
}
?>
<form name="form_prorrogacao" id="form_prorrogacao" method="post" action="" >
    <link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script type="text/javascript">
        function calculaDias(data) {
            var dataAtual = $('dataAtual').value;
            if (data && dataAtual) {
                var myajax = new Ajax.Request('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A', {
                    method: 'post',
                    parameters: '&calculaDiasPAC=true&data=' + data + '&dataAtual=' + dataAtual,
                    asynchronous: false,
                    onComplete: function (res) {
                        if (res.responseText.trim() == 'invalido') {
                            alert('A data informada é inválida.');
                            $('popprazoFinalProrrogacao').value = '';
                            $('qtddias').value = '';
                        } else if (res.responseText.trim() == 'erro') {
                            alert('A data solicitada não pode ser menor que o prazo de vencimento atual.');
                            $('popprazoFinalProrrogacao').value = '';
                            $('qtddias').value = '';
                        } else {
                            $('qtddias').value = res.responseText;
                            $('dias').value = res.responseText;
                        }
                    }
                });
            }
        }
        function salvar() {
            if (!$('popparecer').value) {
                alert('Informe um parecer!');
                return false;
            }
            $('requisicao').value = 'salva';
            $('form_prorrogacao').submit();
        }

        function recusar() {
            if (!$('popparecer').value) {
                alert('Informe um parecer!');
                return false;
            }

            if (confirm('Tem certeza que deseja recusar o prazo dessa Obra?')) {
                $('requisicao').value = 'recusar';
                $('form_prorrogacao').submit();
            }
        }
    </script>
<?php
global $db;

$sql = "SELECT * FROM obras.preobraprorrogacao WHERE popstatus = 'P' AND preid = " . $_REQUEST['preid'];
$prorrogado = $db->pegaLinha($sql);
$prorrogado = $prorrogado ? $prorrogado : Array();

extract($prorrogado);

$sql = "SELECT tooid FROM obras.preobra WHERE preid = {$_REQUEST['preid']}";
$tooid = $db->pegaUm($sql);

// Se a obra for PAC
if ($tooid == 1) {
    $sql = "select 
				MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento,
				MIN(pag.pagdatapagamentosiafi) + 720 as prazo
			from 
				par.pagamentoobra po 
			inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
			where 
				po.preid = " . $_REQUEST['preid'];
} else {// Sea obra não for PAC
    $sql = "SELECT 
				MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento,
				MIN(pag.pagdatapagamentosiafi) + 720 as prazo
			FROM 
				par.pagamentoobrapar po 
			INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
			WHERE 
				po.preid = {$_REQUEST['preid']}";
}


$dataPrimeiroPagamento = $db->carregar($sql);

$sql = "SELECT 
			pre.predescricao, 
			mun.mundescricao || '/' || mun.estuf as entidade ,
			COALESCE(obr.obrpercentultvistoria, 0) as percexec,
			esd.esdid, 
			esd.esddsc as situacao, 
			pto.ptoclassificacaoobra as classificacao,
			CASE 
				WHEN ov.supdtinclusao  IS NOT NULL THEN ov.supdtinclusao
				WHEN obr.obrdtvistoria IS NOT NULL THEN obr.obrdtvistoria
				ELSE obr.obrdtinclusao
			END as ultatualizacao
		FROM 
			obras.preobra pre
		INNER JOIN territorios.municipio 	mun ON mun.muncod = pre.muncod
		LEFT  JOIN obras2.obras obr 
			INNER JOIN workflow.documento 		doc ON doc.docid = obr.docid
			INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc.esdid
		ON obr.preid = pre.preid
		INNER JOIN obras.pretipoobra 		pto ON pto.ptoid = pre.ptoid 
		INNER JOIN workflow.documento 		d   ON d.docid = pre.docid
		INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
		LEFT  JOIN
			(SELECT
				supvid as supervisao, rsuid,obrid, supdtinclusao
			FROM
				obras.supervisao s
			WHERE
				supvid = (SELECT supvid FROM obras.supervisao ss 
						  WHERE ss.supstatus = 'A' AND ss.obrid = s.obrid 
						  ORDER BY supvdt DESC, supvid DESC LIMIT 1)
			) AS ov 
		ON ov.obrid = obr.obrid
		WHERE 
			pre.prestatus = 'A' AND pre.preid = {$_REQUEST['preid']}";

$dadosObra = $db->pegaLinha($sql);

if ($dataPrimeiroPagamento[0]['data_primeiro_pagamento']) {
    $sql = "SELECT popdataprazoaprovado FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid = " . $_REQUEST['preid'];
    $prorrogado = $db->pegaUm($sql);
    
    if ($prorrogado) {
        $dataAtual = $prorrogado;
    } else {
        $dataAtual = $dataPrimeiroPagamento[0]['prazo'];
    }
}

$arrCondicao1 = Array(OBR_ESDID_AGUARDANDO_REGISTRO_DE_PRECOS,
    OBR_ESDID_EM_PLANEJAMENTO_PELO_PROPONENTE,
    OBR_ESDID_EM_LICITACAO);
$arrCondicao2 = Array(OBR_ESDID_EM_REFORMULACAO,
    OBR_ESDID_PARALISADA);

if ($dadosObra['classificacao'] == 'P') {
//	if( $dadosObra['stoid'] == 12 || $dadosObra['stoid'] == 99 || $dadosObra['stoid'] == 5 ){
    if (in_array($dadosObra['esdid'], $arrCondicao1)) {
        $prazoMaximo = "12 Meses";
        $calcula = 12;
//	} elseif( $dadosObra['stoid'] == 1 ){
    } elseif ($dadosObra['esdid'] == OBR_ESDID_EM_EXECUCAO) {
        if ($dadosObra['percexec'] < 26) {
            $prazoMaximo = "12 Meses";
            $calcula = 12;
        } elseif ($dadosObra['percexec'] > 25 && $dadosObra['percexec'] < 51) {
            $prazoMaximo = "9 Meses";
            $calcula = 9;
        } elseif ($dadosObra['percexec'] > 50 && $dadosObra['percexec'] < 76) {
            $prazoMaximo = "6 Meses";
            $calcula = 6;
        } elseif ($dadosObra['percexec'] > 75) {
            $prazoMaximo = "4 Meses";
            $calcula = 4;
        }
//	} elseif( $dadosObra['stoid'] == 9 || $dadosObra['stoid'] == 2 ){
    } elseif (in_array($dadosObra['esdid'], $arrCondicao2)) {
        $prazoMaximo = "Análise Individual";
        $calcula = 0;
    }
} elseif ($dadosObra['classificacao'] == 'Q') {
//	if( $dadosObra['stoid'] == 99 ){
    if ($dadosObra['esdid'] == OBR_ESDID_EM_PLANEJAMENTO_PELO_PROPONENTE) {
        $prazoMaximo = "12 Meses";
        $calcula = 12;
//	} elseif( $dadosObra['stoid'] == 5 ){
    } elseif ($dadosObra['esdid'] == OBR_ESDID_EM_LICITACAO) {
        $prazoMaximo = "9 Meses";
        $calcula = 9;
//	} elseif( $dadosObra['stoid'] == 1 ){
    } elseif ($dadosObra['esdid'] == OBR_ESDID_EM_EXECUCAO) {
        if ($dadosObra['percexec'] < 51) {
            $prazoMaximo = "6 Meses";
            $calcula = 6;
        } elseif ($dadosObra['percexec'] > 50) {
            $prazoMaximo = "3 Meses";
            $calcula = 3;
        }
//	} elseif( $dadosObra['stoid'] == 9 || $dadosObra['stoid'] == 2 ){
    } elseif (in_array($dadosObra['esdid'], $arrCondicao2)) {
        $prazoMaximo = "Análise Individual";
        $calcula = 0;
    }
}
?>
    <input type="hidden" id="dataAtual" name="dataAtual" value="<?php echo $dataAtual; ?>">
    <input type="hidden" id="preid" name="preid" value="<?php echo $_REQUEST['preid']; ?>">
    <input type="hidden" id="requisicao" name="requisicao" value="">
    <input type="hidden" id="dias" name="dias" value="">
    <table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
        <tr>
            <td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
                <?php if ($_REQUEST['recusar'] == 'S') { ?>
                    <b>Recusar Prazo</b> 
                <?php } else { ?>
                    <b>Aceite de Prazo</b>
                <?php } ?>
            </td>
        </tr>
    </table>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td colspan="2">
                <span style="color: red; font-size: 13; text-align: justify;"> 
                    <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
                        <thead>
                            <tr align="center">
                                <td valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Situação da Obra</b></td>
                                <td valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Última Atualização</b></td>
                                <td valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>% Executado</b></td>
                                <td valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Fim da vigência atual</b></td>
                                <td valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>Prazo Máximo a ser concedido após o fim da vigência atual</b></td>
                            </tr>
                        </thead>
                        <tr>
                            <td><?php echo $dadosObra['situacao']; ?></td>
                            <td><?php echo formata_data($dadosObra['ultatualizacao']); ?></td>
                            <td><?php echo $dadosObra['percexec']; ?></td>
                            <td><?php echo formata_data($dataAtual); ?></td>
                            <td><?php echo $prazoMaximo; ?></td>
                        </tr>
                    </table>
                </span>
            </td>
        </tr>
    </table>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <colgroup>
            <col width="40%" />
            <col width="65%" />
        </colgroup>
        <tr>
            <td class="SubTituloDireita">
                Data final do prazo requisitado:
            </td>
            <td>
                <?php echo campo_data2('popdataprazo', 'N', 'N', 'Data final do prazo requisitado', '', '', '', formata_data($popdataprazo), 'calculaDias(this.value)', ''); ?> Dias: <input type="text" id="qtddiassolic" name="qtddiassolic" value="<? echo $popqtddiassolicitado ?>" disabled> <input type="hidden" id="qtddiassolichd" name="qtddiassolichd" value="<? echo $popqtddiassolicitado ?>">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Justificativa:
            </td>
            <td>
                <?php echo $popjustificativa; ?>
            </td>
        </tr>
        <?php if ($_REQUEST['editar']) { ?>
            <tr>
                <td class="SubTituloDireita">
                    Nova Data final do prazo:
                </td>
                <td>
                    <?php echo campo_data2('popprazoFinalProrrogacao', 'S', 'S', 'Data final do prazo requisitado', '', '', 'calculaDias(this.value);', '', 'calculaDias(this.value);', ''); ?> Dias: <input type="text" id="qtddias" name="qtddias" disabled>
                </td>
            </tr>
            <?php } ?>
            <tr>
                <td class="SubTituloDireita">
                    Parecer:
                </td>
                <td>
        <?php echo campo_textarea('popparecer', 'S', 'S', 'Justificativa', 60, 10, 200) ?>
                </td>
            </tr>
            <?php  if($_REQUEST['recusar'] == 'S'){?>
            	<tr>
	                <td class="SubTituloDireita">
	                    Enviar e-mail para entidade?
	                </td>
	                <td>
	        			<input type="checkbox"	id="envia_email" name="envia_email"  value="S" >	
	                </td>
	            </tr>
	         <?php }?>
            <tr align="center">
                <?php if ($_REQUEST['recusar'] == 'S') { ?>
                    <td style="background-color: #dcdcdc" colspan="2"><input type="button" onclick="recusar()" value="Recusar"></td>
                    <?php } else { ?>
                    <td style="background-color: #dcdcdc" colspan="2"><input type="button" onclick="salvar()" value="salvar"></td>
                <?php } ?>
        </tr>
    </table>
</form>