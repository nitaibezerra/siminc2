<?php
if ($_REQUEST['acao'] == 'visualizarPdf') {
    global $db;
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec(null, null, 'par');
    $arqid = $_REQUEST['arqid'];
    $sql = "SELECT * FROM public.arquivo WHERE arqid = {$arqid}";
    $arquivo = $db->carregar($sql);
    
    if($arquivo){
        $arquivo = current($arquivo);
    }else{
        $db->erro = "Arquivo não encontrado.";
        $db->getErroSimec();
        return false;
    }
    $caminho = APPRAIZ.'arquivos/par/'.floor($arqid/1000).'/'.$arqid;
    $filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
    header( 'Content-type: application/pdf' );
    header( 'Content-Disposition: inline; filename='.$filename);
    readfile( $caminho );
    exit();
}

$arqid = $_POST['arqid'];
$id = $_POST['id'];
$arrResultado = array();
$icoid = $_POST['icoid'];
if ($_POST['tipo'] == 'Contrato') {
    $objContrato = new Contratos();
    $arrContrato = $objContrato->findById($id, TRUE);
    $arrResultado = array(
        'numero' => $arrContrato['connumerocontrato'],
        'descricao' => $arrContrato['condescricao'],
        'data' => $arrContrato['condata'],
        'valor'=> number_format($arrContrato['convlrcontrato'], 2, ',', '.'),
        'validado' => ($arrContrato['convalidado'] = TRUE)? 'SIM' : "NÃO",
        'usucpf' => $arrContrato['usucpf']
    );
} else {
    $objNotasFiscais = new NotasFiscais();
    $arrNotaFisca = $objNotasFiscais->findById($id, TRUE);
    $arrResultado = array(
        'numero' => $arrNotaFisca['ntfnumeronotafiscal'],
        'descricao' => $arrNotaFisca['ntfdescricao'],
        'data' => $arrNotaFisca['ntfdata'],
        'valor'=> number_format($arrNotaFisca['ntfvlrnota'], 2, ',', '.'),
        'validado' => ($arrNotaFisca['ntfvalidado'] = TRUE)? 'SIM' : "NÃO",
        'usucpf' => $arrNotaFisca['usucpf']
    );
}
?>
<div class="container">
    <div class="row">
        <div class="span-12">
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <colgroup>
                    <col width="50%" />
                    <col width="50%" />
                </colgroup>
                <tr>
                    <td><strong>NÚMERO: </strong><?php echo $arrResultado['numero']; ?></td>
                    <td><strong>DATA DE INCLUSÃO:</strong> <?php echo $arrResultado['data']; ?></td>
                </tr>
                <tr>
                    <td><strong>DESCRIÇÃO: </strong><?php echo $arrResultado['descricao']; ?></td>
                    <td><strong>VALOR: </strong><?php echo $arrResultado['valor']; ?></td>
                </tr>
                <tr>
                    <td>
                        <strong>ITENS VINCULADOS:</strong>
                        <img style="cursor:pointer" src="/imagens/mais.gif" onclick="visualizarItens(jQuery(this))" title="Visualizar Itens vinculados" border="0">
                    </td>
                    <td>
                        <strong>ID:</strong> <?php echo $id ?>
                    </td>
                </tr>
                <tr id="divItens" style="display: none">
                    <td colspan="2">
                        <?php
                        if ($_POST['tipo'] == 'Contrato') {
                            $sql = "SELECT
                                        si.icodescricao,
                                        ( 
                                            CASE WHEN sbacronograma = 1 
                                                THEN ( 	
                                                    SELECT DISTINCT
                                                        CASE WHEN sic.icovalidatecnico = 'S' 
                                                            THEN sum(coalesce(sic.icoquantidadetecnico,0)) 
                                                        END as vlrsubacao
                                                    FROM par.subacaoitenscomposicao sic
                                                    WHERE
                                                            sic.sbaid = s.sbaid
                                                            AND sic.icoano = sd.sbdano
                                                            and sic.icoid = si.icoid
                                                            AND sic.icostatus = 'A'
                                                    GROUP BY sic.sbaid, sic.icovalidatecnico 
                                                )
                                                ELSE (	
                                                    SELECT DISTINCT
                                                        CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) 
                                                            THEN sum(coalesce(se.sesquantidadetecnico,0))
                                                            ELSE 
                                                                CASE WHEN sic.icovalidatecnico = 'S' 
                                                                    THEN sum(coalesce(ssi.seiqtdtecnico,0))
                                                                END
                                                            END as vlrsubacao
							FROM entidade.entidade t
							INNER JOIN entidade.funcaoentidade 				f   ON f.entid = t.entid
							LEFT  JOIN entidade.entidadedetalhe 			ed  ON t.entid = ed.entid
							INNER JOIN entidade.endereco 					d   ON t.entid = d.entid
							LEFT  JOIN territorios.municipio 				m   ON m.muncod = d.muncod
							LEFT  JOIN par.escolas 							e   ON e.entid = t.entid
							INNER JOIN par.subacaoescolas 					se  ON se.escid = e.escid
							INNER JOIN par.subacaoitenscomposicao 			sic ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
							LEFT  JOIN par.subescolas_subitenscomposicao 	ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
							WHERE
                                                            sic.sbaid = s.sbaid
                                                            AND sic.icoano = sd.sbdano
                                                            AND sic.icoid = si.icoid
                                                            AND (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 --and t.tpcid = v_tpcid
                                                            AND ( CASE WHEN inu.itrid = 2 THEN t.tpcid = 3 ELSE t.tpcid = 1 END ) --Filtra por escolar da esfera da subação.
							GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico
                                                )
                                            END 
                                        ) as quantidade,
                                        si.icoquantidaderecebida
                                    FROM par.subacaoitenscomposicaocontratos sicc
                                    INNER JOIN par.subacaoitenscomposicao si ON si.icoid = sicc.icoid
                                    INNER JOIN par.subacao s ON si.sbaid = s.sbaid  AND si.icostatus = 'A'
                                    INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND si.icoano = sd.sbdano
                                    INNER JOIN par.acao aca on aca.aciid = s.aciid AND acistatus = 'A'
                                    INNER JOIN par.pontuacao pon on pon.ptoid = aca.ptoid AND ptostatus = 'A'
                                    INNER JOIN par.instrumentounidade inu on inu.inuid = pon.inuid
                                    where sicc.conid = {$id}";
                        }else {
                            $sql = "SELECT
                                        si.icodescricao,
                                        ( 
                                            CASE WHEN sbacronograma = 1 
                                                THEN ( 	
                                                    SELECT DISTINCT
                                                        CASE WHEN sic.icovalidatecnico = 'S' 
                                                            THEN sum(coalesce(sic.icoquantidadetecnico,0)) 
                                                        END as vlrsubacao
                                                    FROM par.subacaoitenscomposicao sic
                                                    WHERE
                                                            sic.sbaid = s.sbaid
                                                            AND sic.icoano = sd.sbdano
                                                            and sic.icoid = si.icoid
                                                            AND sic.icostatus = 'A'
                                                    GROUP BY sic.sbaid, sic.icovalidatecnico 
                                                )
                                                ELSE (	
                                                    SELECT DISTINCT
                                                        CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) 
                                                            THEN sum(coalesce(se.sesquantidadetecnico,0))
                                                            ELSE 
                                                                CASE WHEN sic.icovalidatecnico = 'S' 
                                                                    THEN sum(coalesce(ssi.seiqtdtecnico,0))
                                                                END
                                                            END as vlrsubacao
							FROM entidade.entidade t
							INNER JOIN entidade.funcaoentidade 				f   ON f.entid = t.entid
							LEFT  JOIN entidade.entidadedetalhe 			ed  ON t.entid = ed.entid
							INNER JOIN entidade.endereco 					d   ON t.entid = d.entid
							LEFT  JOIN territorios.municipio 				m   ON m.muncod = d.muncod
							LEFT  JOIN par.escolas 							e   ON e.entid = t.entid
							INNER JOIN par.subacaoescolas 					se  ON se.escid = e.escid
							INNER JOIN par.subacaoitenscomposicao 			sic ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
							LEFT  JOIN par.subescolas_subitenscomposicao 	ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
							WHERE
                                                            sic.sbaid = s.sbaid
                                                            AND sic.icoano = sd.sbdano
                                                            AND sic.icoid = si.icoid
                                                            AND (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 --and t.tpcid = v_tpcid
                                                            AND ( CASE WHEN inu.itrid = 2 THEN t.tpcid = 3 ELSE t.tpcid = 1 END ) --Filtra por escolar da esfera da subação.
							GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico
                                                )
                                            END 
                                        ) as quantidade,
                                        si.icoquantidaderecebida
                                    FROM par.subacaoitenscomposicaonotasfiscais sif
                                    INNER JOIN par.subacaoitenscomposicao si ON si.icoid = sif.icoid
                                    INNER JOIN par.subacao s ON si.sbaid = s.sbaid  AND si.icostatus = 'A'
                                    INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND si.icoano = sd.sbdano
                                    INNER JOIN par.acao aca on aca.aciid = s.aciid AND acistatus = 'A'
                                    INNER JOIN par.pontuacao pon on pon.ptoid = aca.ptoid AND ptostatus = 'A'
                                    INNER JOIN par.instrumentounidade inu on inu.inuid = pon.inuid
                                    where sif.ntfid = {$id}";
                        }
                        $cabecalho = array("Descrição do Item", "Quantidade do Termo", "Quantidade recebida");
                        $db->monta_lista_simples($sql,$cabecalho,100,0,'N','95%','N');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td>
                        <strong>HISTÓRICO DE DILIGÊNCIAS DO ITEM:</strong>
                        <img style="cursor:pointer" src="/imagens/mais.gif" onclick="visualizarHistoricoDiligencia(jQuery(this))" title="Visualizar Histórico de Diligência" border="0">
                        <?php
                            $perfil = pegaArrayPerfil($_SESSION['usucpf']);
                            if (
                                in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
                                in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
                                in_array(PAR_PERFIL_PAGADOR, $perfil)
                            ) {
                            ?>
                        <input type="button" style="cursor:pointer" value="Enviar para Diligência" onclick="abrirDiligencia(jQuery(this))" title="Visualizar Histórico de Diligência" border="0">
                            <?php } ?>
                    </td>
                </tr>
                <tr id="divHistorico" style="display: none">
                    <td colspan="2">
                        <?php
                        if ($_POST['tipo'] == 'Contrato') {
                            $sql = "SELECT 
                                        '<img onclick=\"downloadArquivo('|| con.arqid ||');\" style=\"cursor:pointer\" title=\"Download de Contrato\" src=\"../imagens/anexo.gif\">' as acao,
                                        con.conid,
                                        con.connumerocontrato,
                                        con.concpfdiligencia::text,
                                        TO_CHAR(con.condtdiligencia, 'DD/MM/YYYY') as data,
                                        con.condiligencia	
                                    FROM par.subacaoitenscomposicaocontratos sicc
                                    INNER JOIN par.contratos con ON con.conid = sicc.conid
                                    INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid = sicc.icoid
                                    WHERE sicc.icoid = {$icoid} AND sicc.sccstatus = 'A' AND con.condiligencia IS NOT NULL
                                    ORDER BY sccdata DESC";
                            $cabecalho = array("Ações", "ID", "Contrato", "CPF", "Data da Diligência", "Diligência");
                        }else {
                            $sql = "SELECT 
                                        '<img onclick=\"downloadArquivo('|| nf.arqid ||');\" style=\"cursor:pointer\" title=\"Download de Nota Fiscal\" src=\"../imagens/anexo.gif\">' as acao,
                                        nf.ntfid,
                                        nf.ntfnumeronotafiscal,
                                        nf.ntfcpfdiligencia::text,
                                        TO_CHAR(nf.ntfdtdiligencia, 'DD/MM/YYYY') as data,
                                        nf.ntfdiligencia
                                    FROM par.subacaoitenscomposicaonotasfiscais sicnf
                                    INNER JOIN par.notasfiscais nf ON nf.ntfid = sicnf.ntfid
                                    INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid = sicnf.icoid
                                    WHERE sicnf.icoid = {$icoid} AND sicnf.scnstatus = 'A' AND nf.ntfdiligencia IS NOT NULL
                                    ORDER BY ntfdata DESC";
                            $cabecalho = array("Ações", "ID", "Nota Fiscal", "CPF", "Data da Diligência", "Diligência");
                        }
                        
                        $db->monta_lista_simples($sql,$cabecalho,100,0,"N",'95%','N');
                        ?>
                    </td>
                </tr>
                <?php
                if (
                    in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
                    in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
                    in_array(PAR_PERFIL_PAGADOR, $perfil)
                ) {
                ?>
                <tr id="divTxtDiligencia" style="display: none">
                    <td colspan="2">
                        <strong>DILIGÊNCIA:</strong>
                        <div>
                            <textarea name="txDiligencia" id="txDiligencia" style="width: 100%; height: 100px"></textarea>
                            <div style="margin-top: 10px; float: right" class="btn btn-primary" onclick="salvarDiligencia()">Enviar</div>
                        </div>
                    </td>
                </tr>
                <?php } ?>
            </table>
        </div>
    </div>
    <div class="row">
        <div class="span-12">
            <iframe src="par.php?modulo=principal/acompanhamento/visualizarDocumentoPdf&acao=A&acao=visualizarPdf&arqid=<?php echo $arqid?>" width="940px" height="600px" style="border: none;"></iframe>
        </div>
    </div>
</div>
<script>
    function visualizarItens(obj){
        if (jQuery('#divItens').css('display') === 'none') {
            jQuery('#divItens').css('display','');
            obj.attr('title', 'Ocultar Itens vinculados');
            obj.attr('src', '/imagens/menos.gif');
        }else{
            jQuery('#divItens').css('display','none');
            obj.attr('title', 'Visualizar Itens vinculados');
            obj.attr('src', '/imagens/mais.gif');
        }
    }
    function visualizarHistoricoDiligencia(obj){
        if (jQuery('#divHistorico').css('display') === 'none') {
            jQuery('#divHistorico').css('display','');
            obj.attr('title', 'Ocultar Histórico de Diligência');
            obj.attr('src', '/imagens/menos.gif');
        }else{
            jQuery('#divHistorico').css('display','none');
            obj.attr('title', 'Visualizar Histórico de Diligência');
            obj.attr('src', '/imagens/mais.gif');
        }
    }
    function abrirDiligencia(obj){
        if (jQuery('#divTxtDiligencia').css('display') === 'none') {
            jQuery('#divTxtDiligencia').css('display','');
            obj.attr('src', '/imagens/menos.gif');
        }else{
            jQuery('#divTxtDiligencia').css('display','none');
            obj.attr('src', '/imagens/mais.gif');
        }
    }
    function salvarDiligencia() {
        var txDiligencia = jQuery('#txDiligencia').val();
        console.log(txDiligencia);
        if (txDiligencia !== '') {
            jQuery.ajax({
                type: "POST",
                url: window.location.href,
                data: {requisicao: 'salvarDiligencia', txDiligencia: txDiligencia, id: <?php echo $id ?>, tipo: '<?php echo $_POST['tipo'] ?>'},
                async: false,
                success: function (response) {
                    if (response === 'sucesso') {
                        jQuery('#txDiligencia').val('')/
                        jQuery('#divTxtDiligencia').css('display','none');
                        alert('Salvo com Sucesso!');
                        jQuery("#dialog_acoes").dialog( "close" );
                    } else {
                        alert('Falha ao salvar!');
                    }
                }
            });
        } else {
            alert('Campo Obrigatório!');
        }
    }
</script>
<?php exit;