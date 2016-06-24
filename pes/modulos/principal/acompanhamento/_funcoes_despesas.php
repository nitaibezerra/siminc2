
<?php function mostrarDespesasRstantes($coEntidade, $ano){
global $db;
$sql = "SELECT * FROM (
------------------------------------------------------------------------------------------ 1
                select  distinct con.entcodigo, mff.ccdcodigofinanceiro,
                 con.contitulo, lco.lcocodigo as mes, lco.lconome, cec.cecvalor, cec.ccdcodigo, cec.concodigo, tid.tidnome,  tid.tidcodigo, '' as natdescricao, '' as natcodigo,
                    case
                        when coalesce(fisico.fisicopreenchido, 0) = 0 then '0'
                        else 1
                    end as fisicopreenchido
                from pes.pescontrato con
		    LEFT join pes.pescelulacontrato cec on cec.concodigo = con.concodigo
                    inner join pes.pescelulaacompanhamento  cea on cea.concodigo = con.concodigo
                    inner join pes.pesmapafisicofinanceiro  mff on mff.ccdcodigofinanceiro = cea.ccdcodigo
                    inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
		    inner join pes.peslinhacontrato lco on lco.lcocodigo = ccd.lcocodigo
		    inner join pes.pestipodespesa tid on tid.tidcodigo = con.tidcodigo
                    left  join (
                        select
                        ceafis.concodigo, mffis.ccdcodigofinanceiro, count(ceafis.ceavalor) as fisicopreenchido
                        from pes.pescontrato confis
                            inner join pes.pescelulaacompanhamento ceafis on ceafis.concodigo = confis.concodigo
                            inner join pes.pesmapafisicofinanceiro mffis on mffis.ccdcodigofisico = ceafis.ccdcodigo
                        where ceafis.ceaano = {$ano}
                         and confis.entcodigo = {$coEntidade}
                        -- and confis.tidcodigo = 6
                        and ceafis.ceaano = {$ano}
                        and coalesce(ceafis.ceavalor, 0) != 0
                        group by ceafis.concodigo, mffis.ccdcodigofinanceiro
                    ) fisico on fisico.ccdcodigofinanceiro = mff.ccdcodigofinanceiro and fisico.concodigo = cea.concodigo
                where cea.ceaano = {$ano}
                 and con.entcodigo = {$coEntidade}
                -- and con.tidcodigo = 6
                and coalesce(ceavalor, 0) != 0
		and fisico.fisicopreenchido IS NULL
                union
------------------------------------------------------------------------------------------ 1
------------------------------------------------------------------------------------------ 2
                select  distinct con.entcodigo, mff.ccdcodigofinanceiro,
			con.contitulo, lco.lcocodigo as mes, lco.lconome, cec.cecvalor, cec.ccdcodigo, cec.concodigo, tid.tidnome,  tid.tidcodigo, '' as natdescricao, '' as natcodigo,
                    case
                        when coalesce(fisico.fisicopreenchido, 0) = 0 then '0'
                        else 1
                    end as fisicopreenchido
                from pes.pescontrato con
                    inner join pes.pescelulacontrato cec on cec.concodigo = con.concodigo
                    inner join pes.pesmapafisicofinanceiro  mff on mff.ccdcodigofinanceiro = cec.ccdcodigo
                    inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cec.ccdcodigo
                    inner join pes.peslinhacontrato lco on lco.lcocodigo = ccd.lcocodigo
                    inner join pes.pestipodespesa tid on tid.tidcodigo = con.tidcodigo
                    left  join (
                        select
                        cecfis.concodigo, mffis.ccdcodigofinanceiro, count(cecfis.cecvalor) as fisicopreenchido
                        from pes.pescontrato confis
                            inner join pes.pescelulacontrato cecfis on cecfis.concodigo = confis.concodigo
                            inner join pes.pesmapafisicofinanceiro mffis on mffis.ccdcodigofisico = cecfis.ccdcodigo
                        where coalesce(cecfis.cecvalor, 0) != 0
                --      and confis.tidcodigo = 2
                      and confis.entcodigo = {$coEntidade}
                        group by cecfis.concodigo, mffis.ccdcodigofinanceiro
                    ) fisico on fisico.ccdcodigofinanceiro = mff.ccdcodigofinanceiro and fisico.concodigo = cec.concodigo
                where coalesce(cecvalor, 0) != 0
                -- and con.tidcodigo = 2
                 and con.entcodigo = {$coEntidade}
		and fisico.fisicopreenchido IS NULL
                union
------------------------------------------------------------------------------------------ 2
------------------------------------------------------------------------------------------ 3
                select  distinct cnd.entcodigo, can.cancodigo,
			cnd.cndtitulo, lco.lcocodigo as mes, lco.lconome, can.canvalor, can.cancodigo, cnd.cndcodigo, tid.tidnome,  tid.tidcodigo,
			nat.natcodigo || ' - ' || nat.natdescricao as natdescricao, nat.natcodigo as natcodigo,
                    case
                        when coalesce(fisico.fisicopreenchido, 0) = 0 then '0'
                        else 1
                    end as fisicopreenchido
                from pes.pescontratonaturezadespesa cnd
                    inner join pes.pescelulaacompnatdespesa can on can.cndcodigo = cnd.cndcodigo and cantipovalor = 'FN'
                    inner join pes.pestipodespesa tid on tid.tidcodigo = cnd.tidcodigo
		    inner join pes.peslinhacontrato lco on lco.lcocodigo = can.canmes
		    inner join pes.pesnaturezadespesa nat on nat.natcodigo = cnd.natcodigo
                    left join (
                        select cndf.cndcodigo, cndf.entcodigo, cndf.tidcodigo, cndf.unicodigo, cndf.natcodigo, cndf.cndtitulo, canf.cancodigo, canf.canano, canf.canmes, canf.canvalor as fisicopreenchido
                        from pes.pescontratonaturezadespesa cndf
                            inner join pes.pescelulaacompnatdespesa canf on canf.cndcodigo = cndf.cndcodigo and cantipovalor = 'FS'
                        where canf.canano = {$ano}
                --      and cndf.tidcodigo = 12
                      and cndf.entcodigo = {$coEntidade}
                        and coalesce(canf.canvalor, 0) != 0
                    ) fisico on fisico.cndcodigo = cnd.cndcodigo and fisico.unicodigo = cnd.unicodigo and fisico.natcodigo = cnd.natcodigo and fisico.canmes = can.canmes
                where can.canano = {$ano}
                -- and cnd.tidcodigo = 12
                 and cnd.entcodigo = {$coEntidade}
                -- and can.canmes = 1
                and coalesce(can.canvalor, 0) != 0
                and fisico.fisicopreenchido IS NULL
------------------------------------------------------------------------------------------ 3

            ) as despesasgerais
            ORDER BY tidnome, contitulo, mes

            ";
    $despesas = $db->carregar($sql);
//    ver($coEntidade, $sql, $resultado,d);
?>
Algumas despesas estão pendentes de validação, confira na listagem abaixo:
<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;"  > <!--  bgcolor="#f5f5f5"-->
    <thead>
        <tr>
            <td>Tipo do contrato</ td>
            <td>Contrato</ td>
            <td>Mês</ td>
            <td>Ano</ td>
        </ tr>
    </thead>
    <tbody>
        <?php foreach($despesas as $despesa): ?>
            <tr class="list">
                <td><?php echo $despesa['tidnome']?></td>
                <td><?php echo $despesa['contitulo']?></td>
                <td><?php echo $despesa['lconome']?></td>
                <td><?php echo $ano ?></td>
            </tr>
        <?php endforeach ?>
    </tbody>
</table>
<?php } ?>

<?php function montarAcompanhamentoGeral($aConfigs, $anos, $configs, $aAcompanhamento, $aexano) {

    $controllerGeral = new Controller_Geral();
    if($controllerGeral->permission() < 3)
        $save = 'S';
    else
        $save = 'N';

    ?>

    <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem">
        <thead>
            <tr align="center">
                <td  width="120px" rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Mês</strong>
                </td>
                <?php foreach ($anos as $ano) { ?>
                    <td colspan="<?php echo count($configs); ?>" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong><?php echo $ano < $aexano ? 'Série Histórica: <span class="destaque-ano-anterior">' . $ano . '</span>' : 'Ano Exercício: <span class="destaque-ano">' . $ano . '</span>'; ?></strong>
                    </td>
                <?php } ?>
            </tr>
            <tr align="center">
                <?php foreach ($anos as $ano) { ?>
                    <?php foreach ($configs as $config) { ?>
                        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                            <strong><?php echo $config; ?></strong>
                        </td>
                    <?php } ?>
                <?php } ?>
            </tr>
        </thead>
        <tbody>
            <?php
            $count = 0;
            foreach ((array)$aConfigs as $mes => $aConfig) {
                $count++;
                $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
            ?>
                <tr align="center" <?php echo $complemento; ?>>
                    <td align="left"  class="colunaDestaque">
                        <strong>
                        <?php
                        $mes = trim(substr($mes, (strpos($mes, ' - ')+3)));
                        echo $mes;
                        ?>
                        </strong>
                    </td>
                    <?php foreach ($anos as $ano) { ?>
                        <?php foreach ($configs as $ccocodigo => $config) { ?>
                            <td >
                                <?php
                                $ceavalor = $aAcompanhamento[$aConfig[$ccocodigo]['lcocodigo']][$ano][$ccocodigo]['ceavalor'];

                                $casasDecimais = 'IN' == $aConfig[$ccocodigo]['ccdtipovalor'] ? 0 : 2;
                                $ceavalor = $ceavalor ? number_format($ceavalor,$casasDecimais,',','.') : '';

                                $coluna = $ano . '_' . $ccocodigo; ?>
                                <input style="text-align: right;" <?php if($save == 'N') echo 'disabled="disabled"' ?> type="text" class="CampoEstilo soma soma_<?php echo $coluna; ?>" coluna="<?php echo $coluna; ?>" name="ceavalor[<?php echo $ano; ?>][<?php echo $aConfig[$ccocodigo]['ccdcodigo']; ?>]" value="<?php echo $ceavalor; ?>" onKeyUp="<?php echo 'IN' == $aConfig[$ccocodigo]['ccdtipovalor'] ? "this.value=mascaraglobal('[.###]', this.value)" : "this.value=mascaraglobal('[.###],##', this.value)"?>" onFocus="MouseClick(this);this.select();" />
                            </td>
                        <?php } ?>
                    <?php } ?>
                </tr>
            <?php } ?>
            <tr class="colunaDestaque">
                <td><strong>TOTAL</strong></td>
                <?php foreach ($anos as $ano) { ?>
                    <?php foreach ($configs as $ccocodigo => $config) {
                        $coluna = $ano . '_' . $ccocodigo; ?>
                        <td style="text-align: center"><strong><span id="span_<?php echo $coluna; ?>"></span></strong></td>
                    <?php } ?>
                <?php } ?>
            </tr>
            <?php if($save == 'S'): ?>
            <tr id="tr_botoes_acao" style="background-color: #cccccc">
                <td colspan="<?php echo (count($configs) * count($anos)) + 1; ?>">
                    <input type="button" name="botao_gravar" id="botao_gravar" class="botao_gravar" value="Gravar" />
                </td>
            </tr>
            <?php endif ?>
        </tbody>
    </table>

<?php }

function montarAcompanhamentoProcessamentoDados($aConfigs, $anos, $configs, $aAcompanhamento, $aexano)
{
    $controllerGeral = new Controller_Geral();
    if($controllerGeral->permission() < 3)
        $save = 'S';
    else
        $save = 'N';

    global $db;
    rsort($anos);

    $sql = "select * from pes.pesconfigcontratodespesa ccd
                inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
                inner join pes.peslinhacontrato lco on lco.lcocodigo = ccd.lcocodigo
            where ccdtipoconfig = 'CA'
            and tidcodigo = " . K_DESPESA_PROCESSAMENTO_DADOS . "
            -- and substring(cconome from 1 for 1) != '[' -- Pegando os dados sem colchetes (acompanhamento avançado)
            order by lcoordem, ccoordem";

    $configuracao = $db->carregar($sql);

    $aConfigs = array();
    if($configuracao){
        foreach ($configuracao as $config) {
            $configs[$config['ccocodigo']] = $config['cconome'];
            $aConfigs[$config['lcocodigo'] . ' - ' . $config['lconome']][$config['ccocodigo']] = $config;
        }
    }
    ksort($configs);
//    ver($configuracao, $aConfigs, d);
    ?>

            <?php if($save == 'S'): ?>
            <div class="botao_gravar">
                <input type="button" name="botao_gravar_detalhes" id="botao_gravar_detalhes" class="botao_gravar_detalhes" value="Gravar" />
            </div>
            <?php endif ?>

    <?php foreach ($anos as $ano) { ?>

        <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-bottom: 10px;">
            <thead>
                <tr align="center">
                    <td  width="120px" rowspan="4" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Mês</strong>
                    </td>
                    <td colspan="7" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong><?php echo $ano < $aexano ? 'Série Histórica: <span class="destaque-ano-anterior">' . $ano . '</span>' : 'Ano Exercício: <span class="destaque-ano">' . $ano . '</span>'; ?></strong>
                    </td>
                </tr>
                <tr align="center">
                    <td colspan="4" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Desenvolvimento</strong>
                    </td>
                    <td valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Manutenção de Ambientes</strong>
                    </td>
                    <td valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Outros Serviços de TI</strong>
                    </td>
                    <td rowspan="3" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Total (R$)</strong>
                    </td>
                </tr>
                <tr align="center">
                    <td colspan="3" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Ponto de Função</strong>
                    </td>
                    <td rowspan="2" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Valor Total Pago (R$)</strong>
                    </td>
                    <td rowspan="2" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Valor Total Pago (R$)</strong>
                    </td>
                    <td rowspan="2" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Valor Total Pago (R$)</strong>
                    </td>
                </tr>
                <tr align="center">
                    <td valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Nº Contratado</strong>
                    </td>
                    <td valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Nº Utilizado</strong>
                    </td>
                    <td valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Valor Médio Unitário (R$)</strong>
                    </td>
                </tr>
            </thead>
            <tbody>
                <?php
                $count = 0;
                foreach ((array)$aConfigs as $mes => $aConfig) {
                    $count++;
                    $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
                ?>
                    <tr align="center" <?php echo $complemento; ?>>
                        <td align="left"  class="colunaDestaque">
                            <strong>
                            <?php
                            $mes = trim(substr($mes, (strpos($mes, ' - ')+3)));
                            echo $mes;
                            ?>
                            </strong>
                        </td>
                        <?php foreach ($configs as $ccocodigo => $config) { ?>
                            <td>
                                <?php
                                $ceavalor = $aAcompanhamento[$aConfig[$ccocodigo]['lcocodigo']][$ano][$ccocodigo]['ceavalor'];

                                $casasDecimais = 'IN' == $aConfig[$ccocodigo]['ccdtipovalor'] ? 0 : 2;
                                $ceavalor = $ceavalor ? number_format($ceavalor,$casasDecimais,',','.') : '';

                                $coluna      = $ano . '_' . $ccocodigo;
                                $linha       = (in_array($ccocodigo, array(75, 76, 77, 78))) ? $ano . '_' . $aConfig[$ccocodigo]['lcocodigo'] : '';
                                $somaParcial = (in_array($ccocodigo, array(73,74,75))) ? 'parcial_' . $ano . '_' . $aConfig[$ccocodigo]['lcocodigo'] : '';

                                if ('75' == $ccocodigo) { ?>
                                    <?php /*
                                    <span class="soma soma_<?php echo $coluna; ?>" coluna="<?php echo $coluna; ?>"><?php echo $ceavalor; ?></span>
                                    */ ?>
                                    <input <?php if($save == 'N') echo 'disabled="disabled"' ?> style="text-align: right;" type="text" id="<?php echo $somaParcial; ?>" readonly="readonly" class="disabled CampoEstilo soma soma_<?php echo $coluna; ?> <?php echo $linha ? 'linha linha_' . $linha . ' linha_' . $ano : ''; ?>" ano="<?php echo $ano; ?>" coluna="<?php echo $coluna; ?>" linha="<?php echo $linha; ?>" name="ceavalor[<?php echo $ano; ?>][<?php echo $aConfig[$ccocodigo]['ccdcodigo']; ?>]" value="<?php echo $ceavalor; ?>" onKeyUp="this.value=mascaraglobal('[.###],##', this.value)" onFocus="MouseClick(this);this.select();" />
                                <?php } else { ?>
                                    <input <?php if($save == 'N') echo 'disabled="disabled"' ?> style="text-align: right;" type="text" class="CampoEstilo soma soma_<?php echo $coluna; ?> <?php echo $linha ? 'linha linha_' . $linha . ' linha_' . $ano : ''; ?> <?php echo $somaParcial ? 'parcial ' . $somaParcial : ''; ?>"  ano="<?php echo $ano; ?>" coluna="<?php echo $coluna; ?>" linha="<?php echo $linha; ?>" parcial="<?php echo $somaParcial; ?>" name="ceavalor[<?php echo $ano; ?>][<?php echo $aConfig[$ccocodigo]['ccdcodigo']; ?>]" value="<?php echo $ceavalor; ?>" onKeyUp="<?php echo 'IN' == $aConfig[$ccocodigo]['ccdtipovalor'] ? "this.value=mascaraglobal('[.###]', this.value)" : "this.value=mascaraglobal('[.###],##', this.value)"?>" onFocus="MouseClick(this);this.select();" />
                                <?php } ?>
                            </td>
                        <?php } ?>
                        <td style="text-align: right"><strong><span class="soma_total" id="span_<?php echo $linha; ?>"></span></strong></td>
                    </tr>
                <?php } ?>
                <tr class="colunaDestaque">
                    <td><strong>TOTAL</strong></td>
                    <?php foreach ($configs as $ccocodigo => $config) {
                        $coluna = $ano . '_' . $ccocodigo; ?>
                        <td style="text-align: center"><strong><span id="span_<?php echo $coluna; ?>"></span></strong></td>
                    <?php } ?>
                    <td style="text-align: right"><strong><span id="span_total_<?php echo $ano; ?>"></span></strong></td>
                </tr>
            </tbody>
        </table>
    <?php } ?>

            <?php if($save == 'S'): ?>
    <div class="botao_gravar">
        <input type="button" name="botao_gravar_detalhes" id="botao_gravar_detalhes" class="botao_gravar_detalhes" value="Gravar" />
    </div>
            <?php endif ?>

    <?php return $aConfigs;
}

function montarAcompanhamentoTelecomunicacoes($aConfigs, $anos, $configs, $aAcompanhamento, $aexano) {

    $controllerGeral = new Controller_Geral();
    if($controllerGeral->permission() < 3)
        $save = 'S';
    else
        $save = 'N';

    global $db;
    rsort($anos);

    $sql = "select * from pes.pesconfigcontratodespesa ccd
                inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
                inner join pes.peslinhacontrato lco on lco.lcocodigo = ccd.lcocodigo
            where ccdtipoconfig = 'CA'
            and tidcodigo = " . K_DESPESA_TELECOMUNICACOES . "
            and cco.ccocodigo in (28, 29, 30, 31, 32, 33)
            order by lcoordem, ccoordem";

    $configuracao = $db->carregar($sql);

    $aConfigs = array();
    if($configuracao){
        foreach ($configuracao as $config) {
            $configs[$config['ccocodigo']] = $config['cconome'];
            $aConfigs[$config['lcocodigo'] . ' - ' . $config['lconome']][$config['ccocodigo']] = $config;
        }
    }
    ksort($configs);
//    ver($configuracao, $aConfigs, d);
    ?>

            <?php if($save == 'S'): ?>
    <div class="botao_gravar">
        <input type="button" name="botao_gravar_detalhes" id="botao_gravar_detalhes" class="botao_gravar_detalhes" value="Gravar" />
    </div>
            <?php endif ?>

    <?php foreach ($anos as $ano) { ?>

        <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-bottom: 10px;">
            <thead>
                <tr align="center">
                    <td  width="120px" rowspan="3" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Mês</strong>
                    </td>
                    <td colspan="7" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong><?php echo $ano < $aexano ? 'Série Histórica: <span class="destaque-ano-anterior">' . $ano . '</span>' : 'Ano Exercício: <span class="destaque-ano">' . $ano . '</span>'; ?></strong>
                    </td>
                </tr>
                <tr align="center">
                    <td colspan="3" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Telefonia Fixa</strong>
                    </td>
                    <td colspan="2" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Telefonia Móvel</strong>
                    </td>
                    <td rowspan="2" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Internet (R$)</strong>
                    </td>
                    <td rowspan="2" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Total (R$)</strong>
                    </td>
                </tr>
                <tr align="center">
                    <td valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Nº Ramais</strong>
                    </td>
                    <td valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Nº Linhas Diretas</strong>
                    </td>
                    <td valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Total da Fatura (R$)</strong>
                    </td>
                    <td valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Nº Linhas</strong>
                    </td>
                    <td valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Total da Fatura (R$)</strong>
                    </td>
                </tr>
            </thead>
            <tbody>
                <?php
                $count = 0;
                foreach ((array)$aConfigs as $mes => $aConfig) {
                    $count++;
                    $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
                ?>
                    <tr align="center" <?php echo $complemento; ?>>
                        <td align="left"  class="colunaDestaque">
                            <strong>
                            <?php
                            $mes = trim(substr($mes, (strpos($mes, ' - ')+3)));
                            echo $mes;
                            ?>
                            </strong>
                        </td>
                        <?php foreach ($configs as $ccocodigo => $config) { ?>
                            <td>
                                <?php
                                $ceavalor = $aAcompanhamento[$aConfig[$ccocodigo]['lcocodigo']][$ano][$ccocodigo]['ceavalor'];

                                $casasDecimais = 'IN' == $aConfig[$ccocodigo]['ccdtipovalor'] ? 0 : 2;
                                $ceavalor = $ceavalor ? number_format($ceavalor,$casasDecimais,',','.') : '';

                                $coluna      = $ano . '_' . $ccocodigo;
                                $linha       = (in_array($ccocodigo, array(30, 32, 33, 34))) ? $ano . '_' . $aConfig[$ccocodigo]['lcocodigo'] : '';
                                ?>

                                <input <?php if($save == 'N') echo 'disabled="disabled"' ?> style="text-align: right;" type="text" class="CampoEstilo soma soma_<?php echo $coluna; ?> <?php echo $linha ? 'linha linha_' . $linha . ' linha_' . $ano : ''; ?>" ano="<?php echo $ano; ?>" coluna="<?php echo $coluna; ?>" linha="<?php echo $linha; ?>" name="ceavalor[<?php echo $ano; ?>][<?php echo $aConfig[$ccocodigo]['ccdcodigo']; ?>]" value="<?php echo $ceavalor; ?>" onKeyUp="<?php echo 'IN' == $aConfig[$ccocodigo]['ccdtipovalor'] ? "this.value=mascaraglobal('[.###]', this.value)" : "this.value=mascaraglobal('[.###],##', this.value)"?>" onFocus="MouseClick(this);this.select();" />
                            </td>
                        <?php } ?>
                        <td style="text-align: right"><strong><span class="soma_total" id="span_<?php echo $linha; ?>"></span></strong></td>
                    </tr>
                <?php } ?>
                <tr class="colunaDestaque">
                    <td><strong>TOTAL</strong></td>
                    <?php foreach ($configs as $ccocodigo => $config) {
                        $coluna = $ano . '_' . $ccocodigo; ?>
                        <td style="text-align: center"><strong><span id="span_<?php echo $coluna; ?>"></span></strong></td>
                    <?php } ?>
                    <td style="text-align: right"><strong><span id="span_total_<?php echo $ano; ?>"></span></strong></td>
                </tr>
            </tbody>
        </table>
    <?php } ?>

            <?php if($save == 'S'): ?>
    <div class="botao_gravar">
        <input type="button" name="botao_gravar_detalhes" id="botao_gravar_detalhes" class="botao_gravar_detalhes" value="Gravar" />
    </div>
            <?php endif ?>

    <?php return $aConfigs;
}

function montarAcompanhamentoMaterialConsumo($cndcodigo, $anos, $aexano, $entcodigo, $unicod)
{
    $controllerGeral = new Controller_Geral();
    if($controllerGeral->permission() < 3)
        $save = 'S';
    else
        $save = 'N';

    global $db;

    $sql = "select * from pes.pescontratonaturezadespesa cnd
                inner join pes.pesunidademedida uni on uni.unicodigo = cnd.unicodigo
            where cndcodigo = '$cndcodigo'";

    $contratoDespesa = $db->pegaLinha($sql);

    $aConfigs = array(
        '1' => 'Janeiro', '2' => 'Fevereiro', '3' => 'Março',    '4' => 'Abril',   '5' => 'Maio',     '6' => 'Junho',
        '7' => 'Julho',   '8' => 'Agosto',    '9' => 'Setembro', '10'=> 'Outubro', '11'=> 'Novembro', '12'=> 'Dezembro'
    );

    // Dados de acompanhamento gravados no banco
    $sql = "select * from pes.pescelulaacompnatdespesa can
            where canano in (" . implode(', ', $anos) . ")
            and cndcodigo = $cndcodigo";

    $acompanhamento = $db->carregar($sql);


    $aAcompanhamento = array();
    foreach ((array)$acompanhamento as $dado) {
        $aAcompanhamento[$dado['canano']][$dado['canmes']][$dado['cantipovalor']] = $dado['canvalor'];
    }
    ?>

            <?php if($save == 'S'): ?>
    <div class="botao_gravar">
        <input type="button" name="botao_gravar_detalhes" id="botao_gravar_detalhes" class="botao_gravar_detalhes" value="Gravar" />
    </div>
            <?php endif ?>

    <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-bottom: 10px;">
        <thead>
            <tr align="center">
                <td  width="120px" rowspan="2" valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Mês</strong>
                </td>
                <?php foreach ($anos as $ano) { ?>
                    <td colspan="2" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong><?php echo $ano < $aexano ? 'Série Histórica: <span class="destaque-ano-anterior">' . $ano . '</span>' : 'Ano Exercício: <span class="destaque-ano">' . $ano . '</span>'; ?></strong>
                    </td>
                <?php } ?>
            </tr>
            <tr align="center">
                <?php foreach ($anos as $ano) { ?>
                    <td valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong><?php echo (K_UNIDADE_MEDIDA_OUTROS == $contratoDespesa['unicodigo']) ? $contratoDespesa['cndunidadeoutros'] : $contratoDespesa['unititulo']; ?></strong>
                    </td>
                    <td valign="middle" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Realizado (R$)</strong>
                    </td>
                <?php } ?>
            </tr>
        </thead>
        <tbody>
            <?php
            $count = 0;
            foreach ((array)$aConfigs as $mes => $nomeMes) {
                $count++;
                $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
            ?>
                <tr align="center" <?php echo $complemento; ?>>
                    <td align="left"  class="colunaDestaque">
                        <strong>
                        <?php echo $nomeMes; ?>
                        </strong>
                    </td>
                    <?php foreach ($anos as $ano) { ?>
                        <td>
                            <?php
                            $canvalor = $aAcompanhamento[$ano][$mes]['FS'];

                            $casasDecimais = 'IN' == $contratoDespesa['unitipodado'] ? 0 : 2;
                            $canvalor = $canvalor ? number_format($canvalor,$casasDecimais,',','.') : '';
                            ?>
                            <input <?php if($save == 'N') echo 'disabled="disabled"' ?> style="text-align: right;" type="text" class="CampoEstilo soma soma_<?php echo $ano . '_FS'; ?>" coluna="<?php echo $ano . '_FS'; ?>" name="canvalor[<?php echo $ano; ?>][<?php echo $mes; ?>][FS]" value="<?php echo $canvalor; ?>" onKeyUp="<?php echo 'IN' == $contratoDespesa['unitipodado'] ? "this.value=mascaraglobal('[.###]', this.value)" : "this.value=mascaraglobal('[.###],##', this.value)"?>" onFocus="MouseClick(this);this.select();" />
                        </td>
                        <td>
                            <?php
                            $canvalor = $aAcompanhamento[$ano][$mes]['FN'];
                            $canvalor = $canvalor ? number_format($canvalor,2,',','.') : '';
                            ?>
                            <input <?php if($save == 'N') echo 'disabled="disabled"' ?> style="text-align: right;" type="text" class="CampoEstilo soma soma_<?php echo $ano . '_FN'; ?>" coluna="<?php echo $ano . '_FN'; ?>" name="canvalor[<?php echo $ano; ?>][<?php echo $mes; ?>][FN]" value="<?php echo $canvalor; ?>" onKeyUp="this.value=mascaraglobal('[.###],##', this.value)" onFocus="MouseClick(this);this.select();" />
                        </td>
                    <?php } ?>
                </tr>
            <?php } ?>
            <tr class="colunaDestaque">
                <td><strong>TOTAL</strong></td>
                <?php foreach ($anos as $ano) { ?>
                    <td style="text-align: center"><strong><span id="span_<?php echo $ano . '_FS'; ?>"></span></strong></td>
                    <td style="text-align: center"><strong><span id="span_<?php echo $ano . '_FN'; ?>"></span></strong></td>
                <?php } ?>
            </tr>
        </tbody>
    </table>
    <?php if($save == 'S'): ?>
    <div class="botao_gravar">
        <input type="button" name="botao_gravar_detalhes" id="botao_gravar_detalhes" class="botao_gravar_detalhes" value="Gravar" />
    </div>
<?php endif ?>
    <?php

    // where canano in (" . implode(', ', $anos) . ")
    $anoAnterior = $aexano - 1;
    $anos = array($anoAnterior, $aexano);
    $sql = "select
                can.canano,
                can.canmes,
                sum(can.canvalor) as total
            from pes.pescelulaacompnatdespesa can
                inner join pes.pescontratonaturezadespesa cnd on cnd.cndcodigo = can.cndcodigo
            where can.canano in (" . implode(', ', $anos) . ")
            and can.cantipovalor = 'FN'
            -- and cnd.natcodigo = '{$contratoDespesa['natcodigo']}'
            and cnd.tidcodigo = " . K_DESPESA_MATERIAL_CONSUMO . "
            and entcodigo = '$entcodigo'
            group by can.canano, can.canmes";



    $totais = $db->carregar($sql);

    $totalAno = 0;
    $aTotal = array();
    if($totais){
        foreach($totais as $total){
            $aTotal[$total['canano']][$total['canmes']] = $total['total'];
            $totalAno += $total['canano'] == $anoAnterior ? $total['total'] : 0;
        }
    }

    $sql = "select distinct acc.uorcodigo, acc.accvalor, acc.accmes, acc.tidcodigo
            from pes.pesacompconsolidado acc
                inner join pes.pesentidade ent on ent.uorcodigo::character = acc.uorcodigo::character
            where tidcodigo = " . K_DESPESA_MATERIAL_CONSUMO . "
            and accano = $anoAnterior
            and entcodigo = $entcodigo
            and acc.uorcodigo = '$unicod'";
    $totaisConsolidados = $db->carregar($sql);

    $aTotalConsolidado = recuperarRealizadoAno($anoAnterior, $entcodigo, $unicod, K_DESPESA_MATERIAL_CONSUMO);
    $totalAno = array_sum($aTotalConsolidado);

    $aMetaAnual = recuperaMetaPorEntidadeDespesa($unicod, $entcodigo, K_DESPESA_MATERIAL_CONSUMO);

    montaTabelaConsolidadoAlunoEquivalente($unicod, $entcodigo, K_DESPESA_MATERIAL_CONSUMO);
    ?>

    <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
        <thead>
            <tr align="center">
                <td colspan="5" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Consolidação dos Contratos da Despesa - SOF</strong>
                </td>
            </tr>
            <tr align="center">
                <td width="120px" rowspan="2" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Mês</strong>
                </td>
                <?php foreach ($anos as $ano) { ?>
                    <td <?php echo $ano == AEXANO ? 'colspan="3"' : '' ?> valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong><?php echo $ano < $aexano ? 'Série Histórica: <span class="destaque-ano-anterior">' . $ano . '</span>' : 'Ano Exercício: <span class="destaque-ano">' . $ano . '</span>'; ?></strong>
                    </td>
                <?php } ?>
            </tr>
            <tr align="center">
                <?php foreach ($anos as $ano) { ?>
                    <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Realizado (R$)</strong>
                    </td>
                    <?php if($ano == AEXANO){ ?>
                        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                            <strong>Meta de Economia (R$)</strong>
                        </td>
                        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                            <strong>Economia Realizada (R$)</strong>
                        </td>
                    <?php } ?>
                <?php } ?>
            </tr>
        </thead>
        <tbody>
            <?php
            $count = 0;
            $metaTotal = 0;
            $totais = array();
            foreach ((array)$aConfigs as $mes => $nomeMes) {
                $count++;
                $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
            ?>
                <tr align="center" <?php echo $complemento; ?>>
                    <td align="left"  class="colunaDestaque">
                        <strong>
                            <?php
                            echo $nomeMes;
                            ?>
                        </strong>
                    </td>
                    <?php
                    $economia = $aTotalConsolidado[$mes] - $aTotal[$aexano][$mes];
                    $totais[$aexano] += $aTotal[$aexano][$mes];
                    ?>
                    <td align="right" ><?php echo number_format($aTotalConsolidado[$mes], 2, ',', '.'); ?></td>
                    <td align="right" ><?php echo number_format($aTotal[$aexano][$mes], 2, ',', '.'); ?></td>
                    <?php
                    $totais['economia'] += $economia;
                    $metaRealizada = $totalAno ? ($aMetaAnual['valortotal'] / $totalAno) * $aTotalConsolidado[$mes] : 0;
                    $metaTotal += $metaRealizada; ?>

                    <td  align="right"><?php echo number_format($metaRealizada, 2, ',', '.'); ?></td>
                    <td  align="right"><span style="color: <?php echo $economia >= 0 ? 'blue' : 'red'; ?>"><?php echo number_format($economia, 2, ',', '.'); ?></span></td>
                </tr>
            <?php } ?>
            <tr class="colunaDestaque">
                <td><strong>TOTAL</strong></td>
                <td style="text-align: right"><strong><?php echo number_format(array_sum($aTotalConsolidado), 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totais[$aexano], 2, ',', '.'); ?></strong></td>
                <td  align="right"><strong><?php echo number_format($metaTotal, 2, ',', '.'); ?></strong></td>
                <td  align="right"><strong><span style="color: <?php echo $totais['economia']>=0 ? 'blue' : 'red'; ?>"><?php echo number_format($totais['economia'], 2, ',', '.'); ?></span></strong></td>
            </tr>
        </tbody>
    </table>

<?php }

function recuperaMetaPorEntidadeDespesa($unicod, $entcodigo, $tidcodigo = 0)
{
    global $db;

    $sql = "select unscodigo from elabrev.unidadesustentavel where unscodigo = '{$unicod}'";
    $unscodigo = $db->pegaUm($sql);

    if ($unscodigo) {
        $sql = "select  distinct
                    ds.dpsid,
                    ds.dpsdescricao,
                    ls.lcsvalordeducao as valortotal
                from elabrev.despesasustentavel ds
                    left join (
                        select  dpsid,unicod,entid,lcsvalorempenhado,lcsvalormeta,lcsvalordeducao
                        from elabrev.lancamentosustentavel
                        where lcsstatus = 'A' and lcsanoexercicio = '".$_SESSION['exercicio']."'
                    ) ls on ls.dpsid = ds.dpsid and ls.unicod = '$unicod'
                    inner join pes.pesentidade ent on ent.uorcodigo = ls.unicod
                    left join pes.pescontrato con on con.entcodigo = ent.entcodigo and con.tidcodigo = dpsidsispes
                where ds.dpsstatus = 'A'
                and ent.entcodigo = '$entcodigo'
                and dpsidsispes = '$tidcodigo'
                ";
    } else {
        $sql = "select  distinct
                    ds.dpsid,
                    ds.dpsdescricao,
                    (lsaalunoequivalente * lsametavalorreducaoaluno) as valortotal
                From elabrev.despesasustentavel ds
                    left join (
                        select  dpsid, unicod, entid, lsastatus, lsavalorempenhado, lsaalunoequivalente, lsavaloralunoequivalente, lsametareducaoaluno, lsametavalorreducaoaluno
                        from elabrev.lancamentosustentavelaluno
                        where lsastatus = 'A' and lsaanoexercicio = '".$_SESSION['exercicio']."'
                    ) lsa on lsa.dpsid = ds.dpsid and lsa.unicod = '$unicod'
                    inner join pes.pesentidade ent on ent.uorcodigo = lsa.unicod
                    left join pes.pescontrato con on con.entcodigo = ent.entcodigo and con.tidcodigo = dpsidsispes
                where ds.dpsstatus = 'A'
                and lsa.lsastatus = 'A'
                and ent.entcodigo = '$entcodigo'
                and dpsidsispes = '$tidcodigo'
                ";
    }

    return $db->pegaLinha($sql);
}

function recuperaAcompanhamentoAlunoEquivalenteEntidadeDespesa($unicod, $entcodigo, $tidcodigo = 0, $exercicio)
{
    global $db;

    $sql = "select distinct con.tidcodigo, dpsidsispes,
                   ds.dpsdescricao,
                   lsa.lsavalorempenhado,
                   -- (lsa.lsavalorempenhado / 12) as lsavalorempenhadomensal,
                   lsa.lsaalunoequivalente,
                   -- (lsa.lsaalunoequivalente / 12) as lsaalunoequivalentemensal,
                   lsa.lsavalorempenhado/lsa.lsaalunoequivalente as lsavaloralunoequivalente,
                   lsavaloralunoequivalente - lsametavalorreducaoaluno as desppactuadaequival
            from elabrev.despesasustentavel ds
                left join (
                    select  dpsid, unicod, entid, lsastatus, lsavalorempenhado, lsaalunoequivalente, lsavaloralunoequivalente, lsametareducaoaluno, lsametavalorreducaoaluno
                    from elabrev.lancamentosustentavelaluno
                    where lsastatus = 'A' and lsaanoexercicio = '$exercicio'
                ) lsa on lsa.dpsid = ds.dpsid and lsa.unicod = '$unicod'
                inner join pes.pesentidade ent on ent.uorcodigo = lsa.unicod
                left join pes.pescontrato con on con.entcodigo = ent.entcodigo and con.tidcodigo = dpsidsispes
            where ds.dpsstatus = 'A'
            and lsa.lsastatus = 'A'
            and ent.entcodigo = '$entcodigo'
            and dpsidsispes = '$tidcodigo'
            ";

    return $db->pegaLinha($sql);
}

function recuperaAcompanhamentoAlunoEquivalenteEntidade($unicod, $entcodigo, $exercicio)
{
    global $db;

    $sql = "select  sum(lsavalorempenhado) as lsavalorempenhado,
                sum(lsavaloralunoequivalente) as lsavaloralunoequivalente,
                sum(desppactuadaequival) as desppactuadaequival,
                lsaalunoequivalente
            from (
                select distinct con.tidcodigo, dpsidsispes,
                    ds.dpsdescricao,
                    lsa.lsavalorempenhado,
                    lsa.lsaalunoequivalente,
                    lsa.lsavalorempenhado/lsa.lsaalunoequivalente as lsavaloralunoequivalente,
                    lsavaloralunoequivalente - lsametavalorreducaoaluno as desppactuadaequival
                from elabrev.despesasustentavel ds
                    left join (
                        select  dpsid, unicod, entid, lsastatus, lsavalorempenhado, lsaalunoequivalente, lsavaloralunoequivalente, lsametareducaoaluno, lsametavalorreducaoaluno
                        from elabrev.lancamentosustentavelaluno
                        where lsastatus = 'A' and lsaanoexercicio = '$exercicio'
                    ) lsa on lsa.dpsid = ds.dpsid and lsa.unicod = '$unicod'
                    inner join pes.pesentidade ent on ent.uorcodigo = lsa.unicod
                    left join pes.pescontrato con on con.entcodigo = ent.entcodigo and con.tidcodigo = dpsidsispes
                where ds.dpsstatus = 'A'
                and lsa.lsastatus = 'A'
                and ent.entcodigo = '$entcodigo'
            ) as dados
            group by lsaalunoequivalente
            ";
    return $db->pegaLinha($sql);
}

function montaTabelaConsolidadoAlunoEquivalente($unicod, $entcodigo, $tidcodigo)
{
    global $db;

    $sql = "select unscodigo from elabrev.unidadesustentavel where unscodigo = '{$unicod}'";
    $unscodigo = $db->pegaUm($sql);

    if ($unscodigo) {
        return '';
    }
    
    $ano = AEXANO;
    $anoAnterior = ($ano - 1);

    $dadosAEAtual    = recuperaAcompanhamentoAlunoEquivalenteEntidadeDespesa($unicod, $entcodigo, $tidcodigo, $ano);
    $dadosAEAnterior = recuperaAcompanhamentoAlunoEquivalenteEntidadeDespesa($unicod, $entcodigo, $tidcodigo, $anoAnterior);
    
    $despesaAEAtual = $dadosAEAtual['desppactuadaequival'];
    $despesaAEAnterior = $dadosAEAnterior['desppactuadaequival'];    

    if ($tidcodigo == K_DESPESA_MATERIAL_CONSUMO) {
        $sql = "select
                    can.canmes as lcocodigo,
                    sum(can.canvalor) as total
                from pes.pescelulaacompnatdespesa can
                    inner join pes.pescontratonaturezadespesa cnd on cnd.cndcodigo = can.cndcodigo
                where can.canano in ($ano)
                and can.cantipovalor = 'FN'
                and cnd.tidcodigo = " . K_DESPESA_MATERIAL_CONSUMO . "
                and entcodigo = '$entcodigo'
                group by can.canmes";
    } else {
        $sql = "select  lcocodigo,
                        sum(ceavalor) as total
                from pes.pescelulaacompanhamento cea
                    inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
                    inner join pes.pescontrato con on con.concodigo = cea.concodigo
                    inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
                where ceaano in ($ano)
                and con.tidcodigo = $tidcodigo
                and con.entcodigo = $entcodigo
                and ccdtipoconfig = 'CA'
                and ccototaliza = 'S'
                group by lcocodigo";
    }

    $totais = $db->carregar($sql);
    $aExecutado = array();
    if($totais){
        foreach ($totais as $total) {
            $aExecutado[$total['lcocodigo']] = $total['total'];
        }
    }

    $aTotalConsolidado = recuperarRealizadoAno($anoAnterior, $entcodigo, $unicod, $tidcodigo);

    $aConfigs = array(
        '1' => 'Janeiro', '2' => 'Fevereiro', '3' => 'Março',    '4' => 'Abril',   '5' => 'Maio',     '6' => 'Junho',
        '7' => 'Julho',   '8' => 'Agosto',    '9' => 'Setembro', '10'=> 'Outubro', '11'=> 'Novembro', '12'=> 'Dezembro'
    );

    ?>

    <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
        <thead>
            <tr align="center">
                <td colspan="9" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Consolidação dos Contratos da Despesa por Aluno Equivalente - MEC</strong>
                </td>
            </tr>
            <tr align="center">
                <td width="120px" rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Mês</strong>
                </td>
                <td colspan="4" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo 'Série Histórica: <span class="destaque-ano-anterior">' . $anoAnterior . '</span>'; ?></strong>
                </td>
                <td colspan="4" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo 'Ano Exercício: <span class="destaque-ano">' . $ano . '</span>'; ?></strong>
                </td>
            </tr>
            <tr align="center">
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>A = Liquidado (R$)</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>B = Aluno Equivalente</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>C = Despesa por <br />Aluno Equivalente (A/B)</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>D = Despesa pactuada para <?php echo AEXANO; ?> <br /> por aluno equivalente = R$ <?php echo number_format($despesaAEAtual, 2, ',', '.'); ?>    </strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>E = Realizado em 2013</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>F = Aluno Equivalente</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>G = Realizado por <br />Aluno Equivalente (E/F)</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>H = Economia Realizada por <br />Aluno Equivalente (D - G)</strong>
                </td>
            </tr>
        </thead>
        <tbody>
            <?php
            $count = 0;
            $metaTotal = 0;
            $totais = array();
            $totalRealizadoEquivalente = 0;
            $totalEconomia = 0;
            $totalColunaC = 0;

            foreach ((array)$aConfigs as $mes => $nomeMes) {
                $despesaPorAluno  = $dadosAEAtual['lsaalunoequivalente'] ? ($aTotalConsolidado[$mes] / $dadosAEAtual['lsaalunoequivalente']) : '0';
                $totalColunaC    += $despesaPorAluno;
            }

            foreach ((array)$aConfigs as $mes => $nomeMes) {

                $count++;
                $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';

                // Coluna C
                $despesaPorAluno = $dadosAEAtual['lsaalunoequivalente'] ? ($aTotalConsolidado[$mes] / $dadosAEAtual['lsaalunoequivalente']) : '0';

                $despesaPactuada = $totalColunaC ? ($despesaAEAtual / $totalColunaC) * $despesaPorAluno : 0;

                $realizadoEquivalenteG = $dadosAEAtual['lsaalunoequivalente'] ? $aExecutado[$mes] / $dadosAEAtual['lsaalunoequivalente'] : 0;
                $totalRealizadoEquivalente += $realizadoEquivalenteG;

                $economia = $despesaPactuada - $realizadoEquivalenteG;
                $totalEconomia += $economia;
            ?>
                <tr align="center" <?php echo $complemento; ?>>
                    <td align="left"  class="colunaDestaque"><strong> <?php echo $nomeMes; ?> </strong></td>
                    <td align="right" ><?php echo number_format($aTotalConsolidado[$mes], 2, ',', '.'); ?></td>
                    <td align="right" ><?php echo $dadosAEAtual['lsaalunoequivalente']; ?></td>
                    <td align="right" ><?php echo number_format($despesaPorAluno, 2, ',', '.') ?></td>
                    <td align="right" ><?php echo number_format($despesaPactuada, 2, ',', '.') ?></td>
                    <td align="right" ><?php echo number_format($aExecutado[$mes], 2, ',', '.') ?></td>
                    <td align="right" ><?php echo $dadosAEAtual['lsaalunoequivalente']; ?></td>
                    <td align="right" ><?php echo number_format($realizadoEquivalenteG, 2, ',', '.'); ?></td>
                    <td align="right" ><span style="color: <?php echo $economia >= 0 ? 'blue' : 'red'; ?>"><?php echo number_format($economia, 2, ',', '.'); ?></span></td>
                </tr>
            <?php } ?>
            <tr class="colunaDestaque">
                <td><strong>TOTAL</strong></td>
                <td style="text-align: right"><strong><?php echo number_format($dadosAEAtual['lsavalorempenhado'], 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong>&nbsp;</strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalColunaC, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($despesaAEAtual, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format(array_sum($aExecutado), 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong>&nbsp;</strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalRealizadoEquivalente, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><span style="color: <?php echo $totalEconomia >= 0 ? 'blue' : 'red'; ?>"><strong><?php echo number_format($totalEconomia, 2, ',', '.'); ?></strong></span></td>
            </tr>
        </tbody>
    </table>

<?php }

function montarConsolidadoSOF($entcodigo, $unicod, $tidcodigo, $aConfigs)
{
    global $db;
    $aexano = AEXANO;
    $anoAnterior = AEXANO - 1;
    $sql = "select  ceaano,
                    lcocodigo,
                    sum(ceavalor) as total
            from pes.pescelulaacompanhamento cea
                inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
                inner join pes.pescontrato con on con.concodigo = cea.concodigo
                inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
            where ceaano in ($anoAnterior, $aexano)
            and con.tidcodigo = $tidcodigo
            and con.entcodigo = $entcodigo
            and ccdtipoconfig = 'CA'
            and ccototaliza = 'S'
            group by ceaano, lcocodigo";
    $totais = $db->carregar($sql);

    $aTotal = array();
    $totalAno = 0;
    if($totais){
        foreach($totais as $total){
            $aTotal[$total['ceaano']][$total['lcocodigo']] = $total['total'];
        }
    }
    $anos = array($anoAnterior, $aexano);

    $aMetaAnual = recuperaMetaPorEntidadeDespesa($unicod, $entcodigo, $tidcodigo);

    $aTotalConsolidado = recuperarRealizadoAno($anoAnterior, $entcodigo, $unicod, $tidcodigo);
    $totalAno = array_sum($aTotalConsolidado);
    ?>

    <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
        <thead>
            <tr align="center">
                <td colspan="5" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Consolidação dos Contratos da Despesa - SOF</strong>
                </td>
            </tr>
            <tr align="center">
                <td  width="120px" rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Mês</strong>
                </td>
                <?php foreach ($anos as $ano) { ?>
                    <td <?php echo $ano == AEXANO ? 'colspan="3"' : '' ?> valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong><?php echo $ano < $aexano ? 'Série Histórica: <span class="destaque-ano-anterior">' . $ano . '</span>' : 'Ano Exercício: <span class="destaque-ano">' . $ano . '</span>'; ?></strong>
                    </td>
                <?php } ?>
            </tr>
            <tr align="center">
                <?php foreach ($anos as $ano) { ?>
                    <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Realizado (R$)</strong>
                    </td>
                    <?php if($ano == AEXANO){ ?>
                        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                            <strong>Meta de Economia (R$)</strong>
                        </td>
                        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                            <strong>Economia Realizada (R$)</strong>
                        </td>
                    <?php } ?>
                <?php } ?>
            </tr>
        </thead>
        <tbody>
            <?php
            $count     = 0;
            $metaTotal = 0;
            $totais    = array();
            foreach ((array)$aConfigs as $mes => $aConfig) {
                $count++;
                $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
            ?>
                <tr align="center" <?php echo $complemento; ?>>
                    <td align="left"  class="colunaDestaque">
                        <strong>
                            <?php
                            $lcocodigo = trim(substr($mes, 0, strpos($mes, ' - ')));
                            $mes       = trim(substr($mes, (strpos($mes, ' - ')+3)));
                            echo $mes;
                            ?>
                        </strong>
                    </td>
                    <?php
                        $economia = $aTotalConsolidado[$lcocodigo] - $aTotal[$ano][$lcocodigo];
                        $totais[$aexano] += $aTotal[$aexano][$lcocodigo];
                        ?>
                        <td align="right" ><?php echo number_format($aTotalConsolidado[$lcocodigo], 2, ',', '.'); ?></td>
                        <td align="right" ><?php echo number_format($aTotal[$aexano][$lcocodigo], 2, ',', '.'); ?></td>
                        <?php
                        $totais['economia'] += $economia;
                        $metaRealizada = $totalAno ? ($aMetaAnual['valortotal'] / $totalAno) * $aTotalConsolidado[$lcocodigo] : 0;
                        $metaTotal += $metaRealizada; ?>

                        <td  align="right"><?php echo number_format($metaRealizada, 2, ',', '.'); ?></td>
                        <td  align="right"><span style="color: <?php echo $economia >= 0 ? 'blue' : 'red'; ?>"><?php echo number_format($economia, 2, ',', '.'); ?></span></td>
                </tr>
            <?php } ?>
            <tr class="colunaDestaque">
                <td><strong>TOTAL</strong></td>
                <td style="text-align: right"><strong><?php echo number_format(array_sum($aTotalConsolidado), 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totais[$aexano], 2, ',', '.'); ?></strong></td>
                <td  align="right"><strong><?php echo number_format($metaTotal, 2, ',', '.'); ?></strong></td>
                <td  align="right"><strong><span style="color: <?php echo $totais['economia']>=0 ? 'blue' : 'red'; ?>"><?php echo number_format($totais['economia'], 2, ',', '.'); ?></span></strong></td>
            </tr>
        </tbody>
    </table>
<?php }

function recuperarRealizadoAno($ano, $entcodigo, $unicod, $tidcodigo = null)
{
    global $db;

    $sql = "select distinct acc.uorcodigo, acc.accvalor, acc.accmes, acc.tidcodigo
            from pes.pesacompconsolidado acc
                inner join pes.pesentidade ent on ent.uorcodigo::character = acc.uorcodigo::character
            where accano = $ano
            and entcodigo = $entcodigo
            and acc.uorcodigo = '$unicod'";

    $sql .= $tidcodigo ? " and tidcodigo = $tidcodigo " : '';
    $sql .= " order by accmes ";

    $totaisConsolidados = $db->carregar($sql);
    $aTotalConsolidado = array();
    if($totaisConsolidados){
        foreach($totaisConsolidados as $total){
            $aTotalConsolidado[$total['accmes']] += $total['accvalor'];
        }
    }
    return $aTotalConsolidado;
}

function recuperarTotalDespesaEntidadeMes($tidcodigo, $entcodigo)
{
    global $db;

    if (K_DESPESA_MATERIAL_CONSUMO == $tidcodigo) {
        $sql = "select cnd.entcodigo, canano as ano, canmes as mes
                       , sum(can.canvalor) as total
                from pes.pescontratonaturezadespesa cnd
                    inner join pes.pescelulaacompnatdespesa can on can.cndcodigo = cnd.cndcodigo
                where entcodigo = $entcodigo
                and can.cantipovalor = 'FN'
                and cnd.tidcodigo = $tidcodigo
                group by cnd.entcodigo, canano, canmes
                order by canano, canmes";
    } else {
        $sql = "select cont.entcodigo, ceaano as ano, ccd.lcocodigo as mes
                       , sum(cea.ceavalor) as total
                from pes.pescontrato cont
                    inner join pes.pescelulaacompanhamento  cea on cea.concodigo = cont.concodigo
                    inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
                    inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
                where entcodigo = $entcodigo
                and ccototaliza = 'S'
                and cont.tidcodigo = $tidcodigo
                group by cont.entcodigo, ceaano, ccd.lcocodigo
                order by ceaano, ccd.lcocodigo";
    }

    $totais = $db->carregar($sql);

    $aTotal = array();
    if($totais){
        foreach($totais as $total){
            $aTotal[$total['ano']][$total['mes']] = $total['total'];
        }
    }

    return $aTotal;
}

function verificarDiferencaValores($entcodigo, $totalInicial, $totalFinal)
{
    global $db;

    // Comparar diferenças
    $aDiferencas = array();
    foreach ($totalFinal as $anoFinal => $aMesFinal) {
        foreach ($aMesFinal as $mesFinal => $valorFinal) {
            if($valorFinal != $totalInicial[$anoFinal][$mesFinal]){
                $aDiferencas[] = array('mes'=>$mesFinal, 'ano'=>$anoFinal, 'valorInicial'=>$totalInicial[$anoFinal][$mesFinal], 'valorFinal'=>$valorFinal);
            }
        }
    }
    $aEmail = array();
    foreach ($aDiferencas as $diferencas) {
        if ($diferencas['ano'] == AEXANO) {
            $sql = "select * from pes.pesvalidacao
                    where entcodigo = $entcodigo
                    and valmes = {$diferencas['mes']}
                    and valano = {$diferencas['ano']}
                    and tidcodigo = {$_REQUEST['tidcodigo']}
                    ";

            $validacao = $db->pegaLinha($sql);

            if (!empty($validacao['valcodigo']) && 'NI' != trim($validacao['valstatus'])) {

                $sql = "update pes.pesvalidacao set
                            valstatus = 'NI',
                            valvalor = '{$diferencas['valorFinal']}',
                            usucpf = '{$_SESSION['usucpforigem']}',
                            valdata = NOW()
                        where valcodigo = {$validacao['valcodigo']}
                        ";
                $db->executar($sql);
                $db->commit();

                $aEmail[] = $diferencas;
            }
        }
    }
    if($aEmail){
        enviarEmailAlteracaoValores($aEmail, $entcodigo);
    }
}

function enviarEmailAlteracaoValores($aValores, $entcodigo)
{
    global $db;

    // Preparando emails para envio ____________________________________ Envia email
    $emailsDest = array();


    $emailsReme = array('nome' => 'SIMEC - ' . $_SESSION['usunome'], 'email' => $_SESSION['usuemail']);


    $sql = "SELECT usunome, usuemail FROM pes.usuarioresponsabilidade usr
            INNER JOIN seguranca.usuario u on usr.usucpf = u.usucpf
            WHERE usr.entcodigo = $entcodigo
            AND usr.pflcod = " . K_PERFIL_LIDER_UO;

    $responsaveis = $db->carregar($sql);

    if ($responsaveis) {
        foreach($responsaveis as $responsavel){
            // Verifica e-mail para nao ter email repetidos
            if(!in_array($responsavel['usuemail'], $emailsDest)){
                $emailsDest[] = $responsavel['usuemail'];
            }
        }

        if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
            $emailsDest   = array();
            $emailsDest[] = 'ruyjfs@gmail.com';
            $emailsDest[] = $_SESSION['email_sistema'];
        }

        $tidcodigo = $_REQUEST['tidcodigo'] ? $_REQUEST['tidcodigo'] : 0;
        $sql = "select tidnome from pes.pestipodespesa where tidcodigo = $tidcodigo";
        $tidnome = $db->pegaUm($sql);

        $dadosAlterados = '';
        foreach($aValores as $valores){
            $dadosAlterados .= "
                <br />
                Tipo de Despesa: $tidnome
                <br />
                Mês: {$valores['mes']}/{$valores['ano']}
                <br />
                Valor antigo: R$ " . number_format($valores['valorInicial'], 2, ',', '.') . "
                <br />
                Valor atualizado: R$ " . number_format($valores['valorFinal'], 2, ',', '.') . "
                <br />
        ";
        }

        $txt = "Prezado(a) Senhor(a) Líder de UO,
                <br />
                <br />
                Informa-se que os valores validados anteriormente foram alterados parcial ou completamente, pela sua equipe de cadastradores, o que alterou o status do campo de validado para não validado (de verde ou vermelho para branco ou incolor).
                <br />
                <br />
                Pede-se, portanto, que se acesse no módulo \"Esplanada Sustentável\" o menu \"Principal-Validação\" e ratifique novamente o(s) campo(s).
                <br />
                <br />
                Dados alterados:
                $dadosAlterados
                <br />
                <br />
                Atenciosamente,
                <br />
                Equipe Esplanada Sustentável - SIMEC";

        // Envia email aos destinatarios
        enviar_email($emailsReme, $emailsDest, 'SIMEC - Valores validados alterados', $txt , null , array($_SESSION['email_sistema']));
    }
}

function getPreenchimentoFisico($ano = null)
{
    global $db;

    $ano = $ano ? $ano : AEXANO;

    $sql = "select entcodigo, fisicopreenchido, count(*) as qtd
            from (
                select  distinct con.entcodigo, mff.ccdcodigofinanceiro,
                    case
                        when coalesce(fisico.fisicopreenchido, 0) = 0 then '0'
                        else 1
                    end as fisicopreenchido
                from pes.pescontrato con
                    inner join pes.pescelulaacompanhamento  cea on cea.concodigo = con.concodigo
                    inner join pes.pesmapafisicofinanceiro  mff on mff.ccdcodigofinanceiro = cea.ccdcodigo
                    inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
                    left  join (
                        select
                        ceafis.concodigo, mffis.ccdcodigofinanceiro, count(ceafis.ceavalor) as fisicopreenchido
                        from pes.pescontrato confis
                            inner join pes.pescelulaacompanhamento ceafis on ceafis.concodigo = confis.concodigo
                            inner join pes.pesmapafisicofinanceiro mffis on mffis.ccdcodigofisico = ceafis.ccdcodigo
                        where ceafis.ceaano = $ano
                        -- and confis.entcodigo = 669
                        -- and confis.tidcodigo = 6
                        and ceafis.ceaano = $ano
                        and coalesce(ceafis.ceavalor, 0) != 0
                        group by ceafis.concodigo, mffis.ccdcodigofinanceiro
                    ) fisico on fisico.ccdcodigofinanceiro = mff.ccdcodigofinanceiro and fisico.concodigo = cea.concodigo
                where cea.ceaano = $ano
                -- and con.entcodigo = 669
                -- and con.tidcodigo = 6
                and coalesce(ceavalor, 0) != 0

                union

                select  distinct con.entcodigo, mff.ccdcodigofinanceiro,
                    case
                        when coalesce(fisico.fisicopreenchido, 0) = 0 then '0'
                        else 1
                    end as fisicopreenchido
                from pes.pescontrato con
                    inner join pes.pescelulacontrato cec on cec.concodigo = con.concodigo
                    inner join pes.pesmapafisicofinanceiro  mff on mff.ccdcodigofinanceiro = cec.ccdcodigo
                    inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cec.ccdcodigo
                    inner join pes.peslinhacontrato lco on lco.lcocodigo = ccd.lcocodigo
                    inner join pes.pestipodespesa tid on tid.tidcodigo = con.tidcodigo
                    left  join (
                        select
                        cecfis.concodigo, mffis.ccdcodigofinanceiro, count(cecfis.cecvalor) as fisicopreenchido
                        from pes.pescontrato confis
                            inner join pes.pescelulacontrato cecfis on cecfis.concodigo = confis.concodigo
                            inner join pes.pesmapafisicofinanceiro mffis on mffis.ccdcodigofisico = cecfis.ccdcodigo
                        where coalesce(cecfis.cecvalor, 0) != 0
                --      and confis.tidcodigo = 2
                --      and confis.entcodigo = 669
                        group by cecfis.concodigo, mffis.ccdcodigofinanceiro
                    ) fisico on fisico.ccdcodigofinanceiro = mff.ccdcodigofinanceiro and fisico.concodigo = cec.concodigo
                where coalesce(cecvalor, 0) != 0
                -- and con.tidcodigo = 2
                -- and con.entcodigo = 669

                union

                select  distinct cnd.entcodigo, can.cancodigo,
                    case
                        when coalesce(fisico.fisicopreenchido, 0) = 0 then '0'
                        else 1
                    end as fisicopreenchido
                from pes.pescontratonaturezadespesa cnd
                    inner join pes.pescelulaacompnatdespesa can on can.cndcodigo = cnd.cndcodigo and cantipovalor = 'FN'
                    inner join pes.pestipodespesa tid on tid.tidcodigo = cnd.tidcodigo
                    left join (
                        select cndf.cndcodigo, cndf.entcodigo, cndf.tidcodigo, cndf.unicodigo, cndf.natcodigo, cndf.cndtitulo, canf.cancodigo, canf.canano, canf.canmes, canf.canvalor as fisicopreenchido
                        from pes.pescontratonaturezadespesa cndf
                            inner join pes.pescelulaacompnatdespesa canf on canf.cndcodigo = cndf.cndcodigo and cantipovalor = 'FS'
                        where canf.canano = $ano
                --      and cndf.tidcodigo = 12
                --      and cndf.entcodigo = 669
                        and coalesce(canf.canvalor, 0) != 0
                    ) fisico on fisico.cndcodigo = cnd.cndcodigo and fisico.unicodigo = cnd.unicodigo and fisico.natcodigo = cnd.natcodigo and fisico.canmes = can.canmes
                where can.canano = $ano
                -- and cnd.tidcodigo = 12
                -- and cnd.entcodigo = 669
                -- and can.canmes = 1
                and coalesce(can.canvalor, 0) != 0
            ) as despesasgerais
            group by entcodigo, fisicopreenchido
            ";
    $aResultado = $db->carregar($sql);

    $aEntidade = array();
    foreach ($aResultado as $resultado) {
        $aEntidade[$resultado['entcodigo']][$resultado['fisicopreenchido']] = $resultado['qtd'];
        $aEntidade[$resultado['entcodigo']]['qtd'] += $resultado['qtd'];
    }


    return $aEntidade;
}

function recuperaMetaSemAE($unicod, $tidcodigo = null)
{
    global $db;

    if($tidcodigo){
        $sql = "Select
                    (ls.lcsvalorempenhado - ls.lcsvalordeducao ) as totalvalordeducao
                From elabrev.despesasustentavel ds
                Join (
                    Select	unicod,
                                    sum(lcsvalorempenhado) as totalvalorempenhado,
                                    ( (  sum(lcsvalordeducao) / sum(lcsvalorempenhado) ) *100 ) as totalvalormeta,
                                    sum(lcsvalordeducao) as totalvalordeducao
                    From elabrev.lancamentosustentavel
                    Where lcsstatus = 'A' and lcsanoexercicio = '".$_SESSION['exercicio']."'
                    Group by unicod
                ) t on  t.unicod = '{$unicod}'
                Left Join (
                    Select	dpsid,
                                    unicod,
                                    entid,
                                    lcsvalorempenhado,
                                    lcsvalormeta,
                                    lcsvalordeducao
                    From elabrev.lancamentosustentavel
                    Where lcsstatus = 'A' and lcsanoexercicio = '".$_SESSION['exercicio']."'
                ) ls on ls.dpsid = ds.dpsid and ls.unicod = '{$unicod}'

                Where ds.dpsstatus = 'A'
                AND ds.dpsidsispes = {$tidcodigo}";
    } else {
        $sql = "Select DISTINCT (t.totalvalorempenhado - t.totalvalordeducao ) as totalvalordeducao --t.totalvalordeducao as totalvalordeducao
                From elabrev.despesasustentavel ds
                Join (
                    Select	unicod,
                                    sum(lcsvalorempenhado) as totalvalorempenhado,
                                    ( (  sum(lcsvalordeducao) / sum(lcsvalorempenhado) ) *100 ) as totalvalormeta,
                                    sum(lcsvalordeducao) as totalvalordeducao
                    From elabrev.lancamentosustentavel
                    Where lcsstatus = 'A' and lcsanoexercicio = '".$_SESSION['exercicio']."'
                    Group by unicod
                ) t on  t.unicod = '".$unicod."'
                Left Join (
                    Select
                        dpsid,
                        unicod,
                        entid,
                        lcsvalorempenhado,
                        lcsvalormeta,
                        lcsvalordeducao
                    From elabrev.lancamentosustentavel
                    Where lcsstatus = 'A' and lcsanoexercicio = '".$_SESSION['exercicio']."'
                ) ls on ls.dpsid = ds.dpsid and ls.unicod = '".$unicod."'
                Where ds.dpsstatus = 'A'";
    }

    return $db->pegaUm($sql);
}

function montaTabelaConsolidadoUOSemAE($unicod, $entcodigo, $tidcodigo = null)
{
    global $db;

    $metaEconomiaPactuada  = recuperaMetaSemAE($unicod, $tidcodigo);

    $ano = AEXANO;
    $anoAnterior = ($ano - 1);

    $aExecutadoAnterior = recuperaLancadoAno($anoAnterior, $entcodigo , $tidcodigo);
    $aExecutadoAtual    = recuperaLancadoAno($ano, $entcodigo , $tidcodigo);

    $aTotalConsolidado      = recuperarRealizadoAno($anoAnterior, $entcodigo, $unicod , $tidcodigo);
    $aTotalConsolidadoAtual = recuperarRealizadoAno($ano, $entcodigo, $unicod , $tidcodigo);

    $aConfigs = array(
        '1' => 'Janeiro', '2' => 'Fevereiro', '3' => 'Março',    '4' => 'Abril',   '5' => 'Maio',     '6' => 'Junho',
        '7' => 'Julho',   '8' => 'Agosto',    '9' => 'Setembro', '10'=> 'Outubro', '11'=> 'Novembro', '12'=> 'Dezembro'
    );

    ?>
    <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
        <thead>
            <tr align="center">
                <?php if($tidcodigo): ?>
                <?php $nomeTipoDespesa = $db->pegaUm("SELECT tidnome FROM pes.pestipodespesa WHERE tidcodigo = {$tidcodigo};") ?>
                    <td colspan="14" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong style="color: ">Acompanhamento da despesa <font style="color: red"> '<?php echo $nomeTipoDespesa ?>'</font> da UO - MEC</strong>
                    </td>
                <?php else: ?>
                    <td  colspan="14" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="background-color: 69D8FF; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Acompanhamento Consolidado  da UO - MEC</strong>
                    </td>
                <?php endif ?>
            </tr>
            <tr align="center">
                <td width="120px" rowspan="3" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Mês</strong>
                </td>
                <td colspan="6" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>SIAFI (R$)</strong>
                </td>
                <td colspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>SIMEC - PES (R$)</strong>
                </td>
                <td colspan="2" rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>COMPARATIVO<br />SIMEC/SIAFI</strong>
                </td>
            </tr>
            <tr align="center">
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo '<span class="destaque-ano-anterior">' . $anoAnterior . ' (A)</span>'; ?></strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo '<span class="destaque-ano">' . $ano . ' (B)</span>'; ?></strong>
                </td>
                <td rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>¹ C = Meta pactuada <br /> = R$ <?php echo number_format($metaEconomiaPactuada, 2, ',', '.'); ?></strong>
                </td>
                <td rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>D = Economia em Relação ao Ano Anterior <br /> (A - B)</strong>
                </td>
                <td rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>E = Economia em Relação à Meta <br /> (C - B)</strong>
                </td>
                <td rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Sit.</strong>
                </td>

                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo '<span class="destaque-ano-anterior">' . $anoAnterior . ' (F)</span>'; ?></strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo '<span class="destaque-ano">' . $ano . ' (G)</span>'; ?></strong>
                </td>
            </tr>
            <tr align="center">
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>LIQUIDADO</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>LIQUIDADO</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>LANÇADO</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>LANÇADO</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo '<span class="destaque-ano-anterior">' . $anoAnterior . '</span>'; ?></strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo '<span class="destaque-ano">' . $ano . '</span>'; ?></strong>
                </td>

            </tr>
        </thead>
        <tbody>
            <?php
            $count = 0;

            $totalLiquidadaAnoAnterior    = 0;
            $totalLiquidadaAnoAtual       = 0;
            $totalMetaEconomiaPactuadaMes = 0;
            $totalMetaEconomiaPactuadaMeta = 0;
            $totalLancadoAnoAnterior = 0;
            $totalLancadoAnoAtual = 0;

            foreach ((array)$aConfigs as $mes => $nomeMes) {
                $totalLiquidadaAnoAnterior += $aTotalConsolidado[$mes];
            }

            foreach ((array)$aConfigs as $mes => $nomeMes) {

                $count++;
                $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';

                $totalMetaEconomiaPactuadaMes += $metaEconomiaPactuada / $totalLiquidadaAnoAnterior *  $aTotalConsolidado[$mes];
                $totalLiquidadaAnoAtual += $aTotalConsolidadoAtual[$mes];
                $totalLancadoAnoAnterior += $aExecutadoAnterior[$mes];
                $totalLancadoAnoAtual += $aExecutadoAtual[$mes];

                $metaEconomiaPactuadaMes = $metaEconomiaPactuada / $totalLiquidadaAnoAnterior *  $aTotalConsolidado[$mes];

                // D = Economia Realizada por AE (A - B)
                $economiaRealizadaMes = $aTotalConsolidado[$mes] - $aTotalConsolidadoAtual[$mes];

                // E = Economia Realizada por AE (C - B)
                $economiaRealizadaMesMeta = $metaEconomiaPactuadaMes - $aTotalConsolidadoAtual[$mes];
                $totalMetaEconomiaPactuadaMeta += $economiaRealizadaMesMeta;

                $comparativoAnterior = $aTotalConsolidado[$mes]      ? ($aExecutadoAnterior[$mes]/$aTotalConsolidado[$mes])*100 : 0;
                $comparativoAtual    = $aTotalConsolidadoAtual[$mes] ? ($aExecutadoAtual[$mes]/$aTotalConsolidadoAtual[$mes])*100 : 0;

                // Se o valor da despesa for menor ou igual ao da meta, está otimo (verde)
                if ( $aTotalConsolidadoAtual[$mes] <= $metaEconomiaPactuadaMes ) {
                    $imgSituação = '/imagens/icones/bg.png';
                // Se o valor da despesa for maior que a meta, porém houve economia em relação ao ano anterior, merece atenção (amarelo)
                } elseif ($aTotalConsolidadoAtual[$mes] >= $metaEconomiaPactuadaMes && $aTotalConsolidadoAtual[$mes] <= $aTotalConsolidado[$mes]) {
                    $imgSituação = '/imagens/icones/by.png';
                // Se o valor da despesa for maior que a metae que ao ano anterior, está ruim (vermelho)
                } else {
                    $imgSituação = '/imagens/icones/br.png';
                }

            ?>
                <tr align="center" <?php echo $complemento; ?>>
                    <td align="left"  class="colunaDestaque"><strong> <?php echo $nomeMes; ?> </strong></td>
                    <!-- Liquidado 2012 -->
                    <td align="right" ><?php echo number_format($aTotalConsolidado[$mes], 2, ',', '.'); ?></td>
                    <!-- Liquidado 2013 -->
                    <td align="right" ><?php echo number_format($aTotalConsolidadoAtual[$mes], 2, ',', '.') ?></td>
                    <!-- C = Metapactuada  -->
                    <td align="right" ><?php echo number_format($metaEconomiaPactuadaMes, 2, ',', '.') ?></td>
                    <!-- D = Economia Realizada por Ano Anterior (A - B) -->
                    <td align="right" ><span style="color: <?php echo $economiaRealizadaMes >= 0 ? 'blue' : 'red'; ?>"><?php echo $economiaRealizadaMes ? number_format($economiaRealizadaMes, 2, ',', '.') : '-'; ?></span></td>
                    <!-- E = Economia Realizada de acordo com a meta (B - D) -->
                    <td align="right" ><span style="color: <?php echo $economiaRealizadaMesMeta >= 0 ? 'blue' : 'red'; ?>"><?php echo $economiaRealizadaMesMeta ? number_format($economiaRealizadaMesMeta, 2, ',', '.') : '-'; ?></span></td>
                    <!-- Sit. -->
                    <td align="center" ><?php echo $aTotalConsolidadoAtual[$mes] ? "<img src='{$imgSituação}' />" : '-';?></td>
                    <!-- Lançada 2012 -->
                    <td align="right" ><?php echo number_format($aExecutadoAnterior[$mes], 2, ',', '.') ?></td>
                    <!-- Lançada 2013 -->
                    <td align="right" ><?php echo number_format($aExecutadoAtual[$mes], 2, ',', '.') ?></td>
                    <!-- Comparativo sispes -->
                    <td align="right" ><?php echo $comparativoAnterior ? number_format($comparativoAnterior, 2, ',', '.') . ' %' : '-'; ?></td>
                    <!-- Comparativo Siafi -->
                    <td align="right" ><?php echo $comparativoAtual ? number_format($comparativoAtual, 2, ',', '.') . ' %' : '-'; ?></td>
                </tr>
            <?php }

            // Se o valor da despesa for menor ou igual ao da meta, está otimo (verde)
            if ($totalAeAtual <= $despesaAlunoEquivalente) {
                $imgSituação = '/imagens/icones/bg.png';
            // Se o valor da despesa for maior que a meta, porém houve economia em relação ao ano anterior, merece atenção (amarelo)
            } elseif ($totalAeAtual <= $totalAeAnterior) {
                $imgSituação = '/imagens/icones/by.png';
            // Se o valor da despesa for maior que a metae que ao ano anterior, está ruim (vermelho)
            } else {
                $imgSituação = '/imagens/icones/br.png';
            }

            ?>
            <tr class="colunaDestaque">
                <td><strong>TOTAL</strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalLiquidadaAnoAnterior, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalLiquidadaAnoAtual, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalMetaEconomiaPactuadaMes, 2, ',', '.'); ?></strong></td>

                <!--<td style="text-align: right"><strong><?php // echo number_format($totalEconomia, 2, ',', '.'); ?></strong></td>-->
                <td style="text-align: right"><strong><?php echo number_format($totalLiquidadaAnoAnterior - $totalLiquidadaAnoAtual, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalMetaEconomiaPactuadaMes - $totalLiquidadaAnoAtual, 2, ',', '.'); ?></strong></td>

                <td style="text-align: center"><strong><?php echo $totalAeAtual ? "<img src='{$imgSituação}' />" : '-';?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalLancadoAnoAnterior, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalLancadoAnoAtual, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right">
                    <span style="color: <?php echo $totalEconomia >= 0 ? 'blue' : 'red'; ?>">
                        <strong><?php// echo number_format($totalEconomia, 2, ',', '.'); ?></strong>
                    </span>
                </td>
                <td style="text-align: right">
                </td>
            </tr>
        </tbody>
    </table>

    <?php if(!$tidcodigo): ?>
        <br />
        <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
            <tr>
                <td style="border-width: 3px;" style="width: 30%;">
                    <fieldset>
                        <legend>Notas:</legend>
                        <p>¹ C = (Despesa pactuada para <?php echo $ano; ?> / Total A) x "B"</p>
                    </fieldset>
                </td>
                <td style="width: 70%;" valign="top">
                    <p>
                        <!--<b>Observação:</b>-->
                        <br />
                        Meta de economia pactuada: R$ <?php echo number_format($metaEconomiaPactuada, 2, ',', '.'); ?>
                    </p>
                </td>
            </tr>
            <tr>
                <td align="justify">
                    <?php echo substr(textoUltimaCargaSIAFI(), 2); ?>
                </td>
                <td>&nbsp;</td>
            </tr>
        </table>
    <?php endif ?>
<?php }

function montaTabelaConsolidadoUO($unicod, $entcodigo, $tidcodigo = null)
{
    global $db;

    $sql = "select unscodigo from elabrev.unidadesustentavel where unscodigo = '{$unicod}'";
    $unscodigo = $db->pegaUm($sql);

    if ($unscodigo) {
        return montaTabelaConsolidadoUOSemAE($unicod, $entcodigo, $tidcodigo);
    }
    
    $ano = AEXANO;
    $anoAnterior = ($ano-1);

    if($tidcodigo){
        //$dadosAlunoEquivalente = recuperaAcompanhamentoAlunoEquivalenteEntidadeDespesa($unicod, $entcodigo, $tidcodigo);
        $dadosAEAtual = recuperaAcompanhamentoAlunoEquivalenteEntidadeDespesa($unicod, $entcodigo, $tidcodigo, $ano);
        $dadosAEAnterior = recuperaAcompanhamentoAlunoEquivalenteEntidadeDespesa($unicod, $entcodigo, $tidcodigo, $anoAnterior);
    } else {
        //$dadosAlunoEquivalente = recuperaAcompanhamentoAlunoEquivalenteEntidade($unicod, $entcodigo);
        $dadosAEAtual = recuperaAcompanhamentoAlunoEquivalenteEntidade($unicod, $entcodigo, $ano);
        $dadosAEAnterior = recuperaAcompanhamentoAlunoEquivalenteEntidade($unicod, $entcodigo, $anoAnterior);
    }

    //$despesaAlunoEquivalente = $dadosAlunoEquivalente['desppactuadaequival'];
    $despesaAEAtual = $dadosAEAtual['desppactuadaequival'];
    $despesaAEAnterior = $dadosAEAnterior['desppactuadaequival'];

    $aExecutadoAnterior = recuperaLancadoAno($anoAnterior, $entcodigo , $tidcodigo);
    $aExecutadoAtual    = recuperaLancadoAno($ano, $entcodigo , $tidcodigo);

    $aTotalConsolidado      = recuperarRealizadoAno($anoAnterior, $entcodigo, $unicod , $tidcodigo);
    $aTotalConsolidadoAtual = recuperarRealizadoAno($ano, $entcodigo, $unicod , $tidcodigo);

    $aConfigs = array(
        '1' => 'Janeiro', '2' => 'Fevereiro', '3' => 'Março',    '4' => 'Abril',   '5' => 'Maio',     '6' => 'Junho',
        '7' => 'Julho',   '8' => 'Agosto',    '9' => 'Setembro', '10'=> 'Outubro', '11'=> 'Novembro', '12'=> 'Dezembro'
    );

    ?>

    <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
        <thead>
            <tr align="center">
                <?php if($tidcodigo): ?>
                <?php $nomeTipoDespesa = $db->pegaUm("SELECT tidnome FROM pes.pestipodespesa WHERE tidcodigo = {$tidcodigo};") ?>
                    <td colspan="14" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong style="color: ">Acompanhamento da despesa <font style="color: red"> '<?php echo $nomeTipoDespesa ?>'</font> da UO por Aluno Equivalente - MEC</strong>
                    </td>
                <?php else: ?>
                    <td  colspan="14" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="background-color: 69D8FF; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Acompanhamento Consolidado  da UO por Aluno Equivalente - MEC</strong>
                    </td>
                <?php endif ?>
            </tr>
            <tr align="center">
                <td width="120px" rowspan="3" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Mês</strong>
                </td>
                <td colspan="7" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>SIAFI (R$)</strong>
                </td>
                <td colspan="4" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>SIMEC - PES (R$)</strong>
                </td>
                <td colspan="2" rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>COMPARATIVO<br />SIMEC/SIAFI</strong>
                </td>
            </tr>
            <tr align="center">
                <td colspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo '<span class="destaque-ano-anterior">' . $anoAnterior . ' (A)</span>'; ?></strong>
                </td>
                <td colspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo '<span class="destaque-ano">' . $ano . ' (B)</span>'; ?></strong>
                </td>
                <td rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>² C = Meta pactuada por <br /> AE = R$ <?php echo number_format($despesaAEAtual, 2, ',', '.'); ?></strong>
                </td>
                <td rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>D = Economia Realizada <br /> por AE (A - B)</strong>
                </td>
                <td rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Sit.</strong>
                </td>

                <td colspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo '<span class="destaque-ano-anterior">' . $anoAnterior . ' (E)</span>'; ?></strong>
                </td>
                <td colspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo '<span class="destaque-ano">' . $ano . ' (F)</span>'; ?></strong>
                </td>
            </tr>
            <tr align="center">
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Liquidado (R$)</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong> POR AE</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>LIQUIDADO</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong> POR AE³</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>LANÇADO</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong> POR AE</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>LANÇADO</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong> POR AE</strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo '<span class="destaque-ano-anterior">' . $anoAnterior . '</span>'; ?></strong>
                </td>
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong><?php echo '<span class="destaque-ano">' . $ano . '</span>'; ?></strong>
                </td>

            </tr>
        </thead>
        <tbody>
            <?php
            $count = 0;
            $metaTotal = 0;
            $totalAtual = 0;
            $totais = array();
            $totalRealizadoEquivalente = 0;
            $totalRealizadoEquivalenteAtual = 0;
            $totalEconomia = 0;
            $totalAeAnterior = 0;
            $totalAeAtual = 0;

            foreach ((array)$aConfigs as $mes => $nomeMes) {

                $despesaPorAluno  = $dadosAEAnterior['lsaalunoequivalente'] ? ($aTotalConsolidado[$mes] / $dadosAEAnterior['lsaalunoequivalente']) : '0';
                $totalAeAnterior += $despesaPorAluno;

                $despesaPorAlunoAtual  = $dadosAEAtual['lsaalunoequivalente'] ? ($aTotalConsolidadoAtual[$mes] / $dadosAEAtual['lsaalunoequivalente']) : '0';
                $totalAeAtual         += $despesaPorAlunoAtual;
            }

            foreach ((array)$aConfigs as $mes => $nomeMes) {

                $count++;
                $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';

                $despesaPorAluno      = $dadosAEAnterior['lsaalunoequivalente'] ? ($aTotalConsolidado[$mes] / $dadosAEAnterior['lsaalunoequivalente']) : '0';
                $despesaPorAlunoAtual = $dadosAEAtual['lsaalunoequivalente'] ? ($aTotalConsolidadoAtual[$mes] / $dadosAEAtual['lsaalunoequivalente']) : '0';

                $despesaPactuada      = $totalAeAnterior ? ($despesaAEAnterior / $totalAeAnterior) * $despesaPorAluno : 0;
                $despesaPactuadaAtual = $totalAeAtual    ? ($despesaAEAtual / $totalAeAtual) * $despesaPorAlunoAtual : 0;

                $realizadoEquivalenteAnterior = $dadosAEAnterior['lsaalunoequivalente'] ? $aExecutadoAnterior[$mes] / $dadosAEAnterior['lsaalunoequivalente'] : 0;
                $totalRealizadoEquivalente += $realizadoEquivalenteAnterior;

                $realizadoEquivalenteAtual      = $dadosAEAtual['lsaalunoequivalente'] ? $aExecutadoAtual[$mes] / $dadosAEAtual['lsaalunoequivalente'] : 0;
                $totalRealizadoEquivalenteAtual += $realizadoEquivalenteAtual;

                $economia = $despesaPorAlunoAtual ? $despesaPorAluno - $despesaPorAlunoAtual : '0';
                $totalEconomia += $economia;

                $totalAtual += $aTotalConsolidadoAtual[$mes];

                $comparativoAnterior = $aTotalConsolidado[$mes]      ? ($aExecutadoAnterior[$mes]/$aTotalConsolidado[$mes])*100 : 0;
                $comparativoAtual    = $aTotalConsolidadoAtual[$mes] ? ($aExecutadoAtual[$mes]/$aTotalConsolidadoAtual[$mes])*100 : 0;

                // Se o valor da despesa for menor ou igual ao da meta, está otimo (verde)
                if ($despesaPorAlunoAtual <= $despesaPactuada) {
                    $imgSituação = '/imagens/icones/bg.png';
                // Se o valor da despesa for maior que a meta, porém houve economia em relação ao ano anterior, merece atenção (amarelo)
                } elseif ($despesaPorAlunoAtual <= $despesaPorAluno) {
                    $imgSituação = '/imagens/icones/by.png';
                // Se o valor da despesa for maior que a metae que ao ano anterior, está ruim (vermelho)
                } else {
                    $imgSituação = '/imagens/icones/br.png';
                }

            ?>
                <tr align="center" <?php echo $complemento; ?>>
                    <td align="left"  class="colunaDestaque"><strong> <?php echo $nomeMes; ?> </strong></td>
                    <!-- Liquidado 2012 -->
                    <td align="right" ><?php echo number_format($aTotalConsolidado[$mes], 2, ',', '.'); ?></td>
                    <!-- POR AE 2012 -->
                    <td align="right" ><?php echo number_format($despesaPorAluno, 2, ',', '.'); ?></td>
                    <!-- Liquidado 2013 -->
                    <td align="right" ><?php echo number_format($aTotalConsolidadoAtual[$mes], 2, ',', '.') ?></td>
                    <!-- POR AE 2013 -->
                    <td align="right" ><?php echo number_format($despesaPorAlunoAtual, 2, ',', '.') ?></td>
                    <!-- C = Metapactuada por AE =  -->
                    <td align="right" ><?php echo number_format($despesaPactuada, 2, ',', '.') ?></td>
                    <!-- POR AE D = Economia Realizada por AE (A - B) -->
                    <td align="right" ><span style="color: <?php echo $economia >= 0 ? 'blue' : 'red'; ?>"><?php echo $economia ? number_format($economia, 2, ',', '.') : '-'; ?></span></td>
                    <!-- Sit. -->
                    <td align="center" ><?php echo $despesaPorAlunoAtual ? "<img src='{$imgSituação}' />" : '-';?></td>
                    <!-- Lançada 2012 -->
                    <td align="right" ><?php echo number_format($aExecutadoAnterior[$mes], 2, ',', '.') ?></td>
                    <!-- POR AE 2012 -->
                    <td align="right" ><?php echo $realizadoEquivalenteAnterior ? number_format($realizadoEquivalenteAnterior, 2, ',', '.') : '-'; ?></td>
                    <!-- Lançada 2013 -->
                    <td align="right" ><?php echo number_format($aExecutadoAtual[$mes], 2, ',', '.') ?></td>
                    <!-- POR AE 2013 -->
                    <td align="right" ><?php echo $realizadoEquivalenteAtual ? number_format($realizadoEquivalenteAtual, 2, ',', '.') : '-'; ?></td>
                    <!-- Comparativo sispes -->
                    <td align="right" ><?php echo $comparativoAnterior ? number_format($comparativoAnterior, 2, ',', '.') . ' %' : '-'; ?></td>
                    <!-- Comparativo Siafi -->
                    <td align="right" ><?php echo $comparativoAtual ? number_format($comparativoAtual, 2, ',', '.') . ' %' : '-'; ?></td>
                </tr>
            <?php }

            // Se o valor da despesa for menor ou igual ao da meta, está otimo (verde)
            if ($totalAeAtual <= $despesaAEAtual) {
                $imgSituação = '/imagens/icones/bg.png';
            // Se o valor da despesa for maior que a meta, porém houve economia em relação ao ano anterior, merece atenção (amarelo)
            } elseif ($totalAeAtual <= $totalAeAnterior) {
                $imgSituação = '/imagens/icones/by.png';
            // Se o valor da despesa for maior que a metae que ao ano anterior, está ruim (vermelho)
            } else {
                $imgSituação = '/imagens/icones/br.png';
            }

            ?>
            <tr class="colunaDestaque">
                <td><strong>TOTAL</strong></td>
                <td style="text-align: right"><strong><?php echo number_format($dadosAlunoEquivalente['lsavalorempenhado'], 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalAeAnterior, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalAtual, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalAeAtual, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($despesaAEAtual, 2, ',', '.'); ?></strong></td>

                <!--<td style="text-align: right"><strong><?php // echo number_format($totalEconomia, 2, ',', '.'); ?></strong></td>-->
                <td style="text-align: right"><strong><?php echo number_format($totalAeAnterior - $totalAeAtual, 2, ',', '.'); ?></strong></td>

                <td style="text-align: center"><strong><?php echo '-';//$totalAeAtual ? "<img src='{$imgSituação}' />" : '-';?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format(array_sum($aExecutadoAnterior), 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalRealizadoEquivalente, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format(array_sum($aExecutadoAtual), 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><strong><?php echo number_format($totalRealizadoEquivalenteAtual, 2, ',', '.'); ?></strong></td>
                <td style="text-align: right"><span style="color: <?php echo $totalEconomia >= 0 ? 'blue' : 'red'; ?>">
                        <strong><?php// echo number_format($totalEconomia, 2, ',', '.'); ?></strong></span>
                </td>
            </tr>
        </tbody>
    </table>

    <?php if(!$tidcodigo):?>
        <br />
        <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
            <tr>
                <td style="border-width: 3px;" style="width: 30%;">
                    <fieldset>
                        <legend>Notas:</legend>
                        <p>¹ Aluno Equivalente utilizado para a elaboração do orçamento de <?php echo $ano ?> </p>
                        <p> ² C = (Despesa liquidada, em <?php echo $ano - 1; ?>, por AE / Total A ? Por AE) x "Meta de economia pactuada por AE"</p>
                        <p>³ Será tualizado o número "Aluno Equivalente", no mês provável de Agosto/<?php echo $ano; ?></p>
                    </fieldset>
                </td>
                <td style="width: 70%;" valign="top">
                    <p>
                        <!--<b>Observação:</b>-->
                        <br />
                        ALUNO EQUIVALENTE (AE) - <?php echo $anoAnterior ?>: <?php echo $dadosAEAnterior['lsaalunoequivalente']; ?>
                        <br />
                        <br />
                        ALUNO EQUIVALENTE (AE) - <?php echo $ano ?>: <?php echo $dadosAEAtual['lsaalunoequivalente']; ?>
                        <br />
                        <br />
                        Meta de economia pactuada por AE: R$ <?php echo number_format($despesaAEAtual, 2, ',', '.'); ?>
                    </p>
                </td>
            </tr>
            <tr>
                <td align="justify">
                    <?php echo substr(textoUltimaCargaSIAFI(), 2); ?>
                </td>
                <td>&nbsp;</td>
            </tr>
        </table>
    <?php endif ?>
<?php }

function montaTabelaPainelExecucaoResumida( $aexano = null)
{
    global $db;

    if(!$aexano) $aexano = AEXANO;

    $sql = "select  distinct  ent.uorcodigo, ent.entcodigo, ent.entnome, ent.aexano, acc.valor as execucao,
                    lsa.lsaalunoequivalente,
                    ( acc.valor / lsaalunoequivalente ) as pactuada , desppactuadaequival,
                    (select count(*) from elabrev.unidadesustentavel where unscodigo = ent.uorcodigo::integer) as unidadesustentavel
                    , CASE WHEN desppactuadaequival > 0 THEN (  ( acc.valor / lsaalunoequivalente ) / desppactuadaequival) * 100 ELSE 0 END as percentual
                    -- lsa.lsavalorempenhado, lsa.lsavaloralunoequivalente, lsa.desppactuadaequival
            from pes.pesentidade ent
                            inner join pes.pesunidadeorcamentaria uor on uor.uorcodigo = ent.uorcodigo
                            left join(
                                           select ac.uorcodigo, sum(ac.accvalor) as valor
                                           from pes.pesacompconsolidado ac
                                           where accano = {$aexano}
                                           group by ac.uorcodigo
                            ) as acc on acc.uorcodigo = ent.uorcodigo
                    left join (
                            select entcodigo, lsaalunoequivalente,
                                            sum(lsavalorempenhado) as lsavalorempenhado,
                                            sum(lsavalorempenhado/lsaalunoequivalente) as lsavaloralunoequivalente,
                                            sum(lsavaloralunoequivalente - lsametavalorreducaoaluno) as desppactuadaequival
                            from (
                                            select distinct
                                                            ent.entcodigo,
                                                            con.tidcodigo, dpsidsispes,
                                                   ds.dpsdescricao,
                                                   lsa.lsavalorempenhado,
                                                   lsa.lsaalunoequivalente,
                                                   lsametavalorreducaoaluno,
                                                   lsa.lsavalorempenhado/lsa.lsaalunoequivalente as lsavaloralunoequivalente,
                                                   lsavaloralunoequivalente - lsametavalorreducaoaluno as desppactuadaequival
                                            from elabrev.despesasustentavel ds
                                                left join (
                                                            select  dpsid, unicod, entid, lsastatus, lsavalorempenhado, lsaalunoequivalente, lsavaloralunoequivalente, lsametareducaoaluno, lsametavalorreducaoaluno
                                                            from elabrev.lancamentosustentavelaluno
                                                            where lsastatus = 'A' and lsaanoexercicio = '".$_SESSION['exercicio']."'
                                                ) lsa on lsa.dpsid = ds.dpsid -- and lsa.unicod = '26256'
                                                inner join pes.pesentidade ent on ent.uorcodigo = lsa.unicod
                                                left join pes.pescontrato con on con.entcodigo = ent.entcodigo and con.tidcodigo = dpsidsispes
                                            where ds.dpsstatus = 'A'
                                            and lsa.lsastatus = 'A'
                                            -- and ent.entcodigo = '669'
                            ) as foo
                            group by entcodigo, lsaalunoequivalente
                    ) lsa on lsa.entcodigo = ent.entcodigo
            where uor.orgcodigo = '26000'
            and ent.aexano = {$aexano}
            -- and ent.uorcodigo = '26256'
            group by ent.uorcodigo, ent.entcodigo, ent.entnome, ent.aexano, acc.valor, lsaalunoequivalente, desppactuadaequival
            order by entnome";

            $result = $db->carregar($sql);

//            ver($result,d);
            ?>
    <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
            <thead>
                <tr align="center">
                    <td  colspan="14" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="background-color: 69D8FF; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Atualização de comprimento de meta pactuada <?php echo $aexano ?></strong>
                    </td>
                </tr>
                <tr align="center">
                    <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Despesa</strong>
                    </td>
                    <td rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Pactuada <?php echo $ano ?></strong>
                    </td>
                    <td rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Execução Real <?php echo $ano ?></strong>
                    </td>
                    <td rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                        <strong>Percentual</strong>
                    </td>
                </tr>
            </thead>
            <tbody>
                <?php if($result):
                    foreach ($result as $key => $value):
                        $count++;
                        $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';

                        if(!$value['unidadesustentavel'] ):
                        ?>
                        <tr <?php echo $complemento; ?>>
                            <td style="text-align: left"><strong><?php echo $value['entnome'] ?></strong></td>
                            <td style="text-align: right"><strong><?php echo number_format($value['desppactuadaequival'], 2, ',', '.'); ?></strong></td>
                            <td style="text-align: right"><strong><?php echo number_format($value['pactuada'], 2, ',', '.'); ?></strong></td>
                            <td style="text-align: right"><strong <?php if($value['percentual'] > 100) echo 'style="color: red"' ?>><?php echo number_format($value['percentual'], 2, ',', '.'); ?>%</strong></td>
                        </tr>
                    <?php endif;
                    endforeach;
                endif;
                ?>
            </tbody>
        </table>
        <br />
        <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
            <tr>
                <th colspan="2" style="text-align: left;">Notas:</th>
            </tr>
            <tr>
                <td align="justify" style="border:thin; border-style:solid; border-color:#BEBEBE;width:50%">
                    <?php echo textoUltimaCargaSIAFI(); ?>
                </td>
                <td>&nbsp;</td>
            </tr>
        </table>
<?php




//            ver($sql , $result,d);

}

function montaTabelaConsolidadoUOPainelAluno($unicod, $entcodigo)
{
    global $db;

    $sql = "select unscodigo from elabrev.unidadesustentavel where unscodigo = '{$unicod}'";
    $unscodigo = $db->pegaUm($sql);

    if ($unscodigo) {
        $ano = AEXANO;
        $anoAnterior = ($ano - 1);

        $sql = "select tid.tidcodigo, tid.tidnome, meta.pactuada, exe.executado as execucao 
                , CASE WHEN meta.pactuada > 0 THEN ((exe.executado / meta.pactuada ) * 100) ELSE 0 END as percentual
                from pes.pestipodespesa tid
                    left join (
                            Select ds.dpsidsispes, (ls.lcsvalorempenhado - ls.lcsvalordeducao ) as pactuada
                            From elabrev.despesasustentavel ds
                            Join (
                                Select unicod, sum(lcsvalorempenhado) as totalvalorempenhado,( (  sum(lcsvalordeducao) / sum(lcsvalorempenhado) ) *100 ) as totalvalormeta, sum(lcsvalordeducao) as totalvalordeducao
                                From elabrev.lancamentosustentavel
                                Where lcsstatus = 'A' and lcsanoexercicio = '".$_SESSION['exercicio']."'
                                Group by unicod
                            ) t on  t.unicod = '{$unicod}'
                            Left Join (
                                Select dpsid,unicod,entid,lcsvalorempenhado,lcsvalormeta,lcsvalordeducao
                                From elabrev.lancamentosustentavel
                                Where lcsstatus = 'A' and lcsanoexercicio = '".$_SESSION['exercicio']."'
                            ) ls on ls.dpsid = ds.dpsid and ls.unicod = '{$unicod}'
                            Where ds.dpsstatus = 'A'
                    ) meta on meta.dpsidsispes = tid.tidcodigo

                    left join (
                        select  con.tidcodigo, sum(ceavalor) as executado
                        from pes.pescelulaacompanhamento cea
                                        inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
                                        inner join pes.pescontrato con on con.concodigo = cea.concodigo
                                        inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
                        where ceaano = {$ano}
                        and con.entcodigo = {$entcodigo}
                        and ccdtipoconfig = 'CA'
                        and ccototaliza = 'S'
                        group by con.tidcodigo

                        union

                        select
                                        12 as tidcodigo,
                                        sum(can.canvalor) as executado
                        from pes.pescelulaacompnatdespesa can
                                        inner join pes.pescontratonaturezadespesa cnd on cnd.cndcodigo = can.cndcodigo
                        where can.canano = {$ano}
                        and can.cantipovalor = 'FN'
                        and cnd.tidcodigo = 12
                        and entcodigo = '{$entcodigo}'
                    ) exe on exe.tidcodigo = tid.tidcodigo
                where tidacompanha = 't'
                and tidpossuimeta = 't'
                order by tid.tidnome";

        $result = $db->carregar($sql);
    } else {
        $ano = AEXANO;
        $anoAnterior = ($ano - 1);
        $sql = "select distinct accano, acc.uorcodigo, acc.tidcodigo, tid.tidnome
            --, sum(acc.accvalor) as execucao 
            ,  lsaalunoequivalente 
            , ( sum(acc.accvalor) / lsaalunoequivalente ) as execucao 
            , desppactuadaequival as pactuada 
            , CASE WHEN desppactuadaequival > 0 THEN ((( sum(acc.accvalor) / lsaalunoequivalente ) / desppactuadaequival ) * 100) ELSE 0 END as percentual
            from pes.pesacompconsolidado acc
            inner join pes.pesentidade ent on ent.uorcodigo::character = acc.uorcodigo::character
            inner join pes.pestipodespesa tid on acc.tidcodigo = tid.tidcodigo
            left join (select distinct con.tidcodigo, dpsidsispes,
                               ds.dpsdescricao,
                               lsa.lsavalorempenhado,
                               -- (lsa.lsavalorempenhado / 12) as lsavalorempenhadomensal,
                               lsa.lsaalunoequivalente,
                               -- (lsa.lsaalunoequivalente / 12) as lsaalunoequivalentemensal,
                               lsa.lsavalorempenhado/coalesce(lsa.lsaalunoequivalente, 1) as lsavaloralunoequivalente,
                               lsavaloralunoequivalente - lsametavalorreducaoaluno as desppactuadaequival
                        from elabrev.despesasustentavel ds
                            left join (
                                select  dpsid, unicod, entid, lsastatus, lsavalorempenhado, lsaalunoequivalente, lsavaloralunoequivalente, lsametareducaoaluno, lsametavalorreducaoaluno
                                from elabrev.lancamentosustentavelaluno
                                where lsastatus = 'A' and lsaanoexercicio = '".$_SESSION['exercicio']."'
                            ) lsa on lsa.dpsid = ds.dpsid and lsa.unicod = '{$unicod}'
                            inner join pes.pesentidade ent on ent.uorcodigo = lsa.unicod
                            left join pes.pescontrato con on con.entcodigo = ent.entcodigo and con.tidcodigo = dpsidsispes
                        where ds.dpsstatus = 'A'
                        and lsa.lsastatus = 'A'
                        and ent.entcodigo = '{$entcodigo}') aluequivalente on aluequivalente.dpsidsispes = acc.tidcodigo
            where accano =  {$ano}
            and entcodigo = {$entcodigo}
            and acc.uorcodigo = '{$unicod}'
            -- and tidcodigo = 6
            group by accano, acc.uorcodigo,tid.tidnome, acc.tidcodigo , lsaalunoequivalente , desppactuadaequival
            order by accano, tid.tidnome";
    //            ver($sql,d);

        $result = $db->carregar($sql);
    }
//        ver($result);



    ?>

    <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
        <thead>
            <tr align="center">
                <td  colspan="14" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="background-color: 69D8FF; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Atualização de Comprimento de Metas Pactuadas para <?php echo $ano ?></strong>
                </td>
            </tr>
            <tr align="center">
                <td onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Despesa</strong>
                </td>
                <td rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Pactuada <?php echo $ano ?></strong>
                </td>
                <td rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Execução <?php echo $ano ?></strong>
                </td>
                <td width="110" rowspan="2" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Percentual</strong>
                </td>
            </tr>
        </thead>
        <tbody>
            <?php

                $totalPactuada = 0;
                $totalExecucao = 0;
            if($result):
                foreach ($result as $key => $value):

                    $totalPactuada += $value['pactuada'];
                    $totalExecucao += $value['execucao'];
//                    $totalPercentual += $value['pactuada'];

                $count++;
                $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
                $precentVal = number_format($value['percentual'], 2, ',', '.');

                ?>
                <tr <?php echo $complemento; ?>>
                    <td style="text-align: left"><strong><?php echo $value['tidnome'] ?></strong></td>
                    <td style="text-align: right"><strong><?php echo number_format($value['pactuada'], 2, ',', '.'); ?></strong></td>
                    <td style="text-align: right"><strong><?php echo number_format($value['execucao'], 2, ',', '.'); ?></strong></td>
                    <td style="text-align: center"><strong><?php echo showProgressBar($precentVal); ?></strong></td>
                </tr>
            <?php endforeach;
                endif;
                ?>
                <tr style="background-color: #DCDCDC;">
                    <td>
                        <strong>Total: </strong>
                    </td>
                    <td style="text-align: right">
                        <strong><?php echo number_format( $totalPactuada , 2, ',', '.'); ?></strong>
                    </td>
                    <td style="text-align: right">
                        <strong><?php echo number_format( $totalExecucao , 2, ',', '.'); ?></strong>
                    </td>
                    <td style="text-align: right">
                        <strong ><?php //echo ( count($totalPactuada) > 0 & count($totalExecucao) > 0)? number_format( $totalPactuada / $totalExecucao * 100 , 2, ',', '.') : 0; ?></strong>
                    </td>
                </tr>
        </tbody>
    </table>
    <br />
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
        <tr>
            <td style="border-width: 3px;" style="width: 30%;">
                <fieldset>
                    <legend>Notas:</legend>
                    <?php echo textoUltimaCargaSIAFI(); ?>
                </fieldset>
            </td>
        </tr>
    </table>
<?php }

/**
 * Retonar barra de progresso com porcentagem
 * @param float $precentVal
 * @return string HTML
 */
function showProgressBar($precentVal)
{
//    $precentVal = ($precentVal > 100) ? 100 : $precentVal;
    $precentValCss = ($precentVal > 100) ? 100 : $precentVal;
    $markupPb = '<div id="progress-bar" class="all-rounded">';
        if($precentVal > 100){
            $markupPb.= '<div id="progress-bar-percentage-red" class="all-rounded" style="width: %spx;">%s</div></div>';
        } else {
            $markupPb.= '<div id="progress-bar-percentage" class="all-rounded" style="width: %spx;">%s</div></div>';
        }

    if ($precentVal > 10) {
        $complement = "&nbsp;$precentVal%";
    } else {
            $styleRule = 'position:absolute;color:#5D9E86;margin-top:-14px;margin-left:'.($precentVal).'px;';

        $complement = "<div class=\"spacer\">&nbsp;</div><div style=\"{$styleRule}\">{$precentVal}%</div>";
    }

    return sprintf($markupPb, (int)$precentValCss, $complement);
}


function recuperaLancadoAno($ano, $entcodigo, $tidcodigo = null)
{
    global $db;

    if ($tidcodigo) {
        if ($tidcodigo == K_DESPESA_MATERIAL_CONSUMO) {
            $sql = "select
                        can.canmes as lcocodigo,
                        sum(can.canvalor) as total
                    from pes.pescelulaacompnatdespesa can
                        inner join pes.pescontratonaturezadespesa cnd on cnd.cndcodigo = can.cndcodigo
                    where can.canano in ($ano)
                    and can.cantipovalor = 'FN'
                    and cnd.tidcodigo = " . K_DESPESA_MATERIAL_CONSUMO . "
                    and entcodigo = '$entcodigo'
                    group by can.canmes";
        } else {
            $sql = "select  lcocodigo,
                            sum(ceavalor) as total
                    from pes.pescelulaacompanhamento cea
                        inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
                        inner join pes.pescontrato con on con.concodigo = cea.concodigo
                        inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
                    where ceaano in ($ano)
                    and con.tidcodigo = $tidcodigo
                    and con.entcodigo = $entcodigo
                    and ccdtipoconfig = 'CA'
                    and ccototaliza = 'S'
                    group by lcocodigo";
        }
    } else {

        $sql = "select  lcocodigo,
                    sum(total) as total
                from(
                    select  lcocodigo,
                        sum(ceavalor) as total
                    from pes.pescelulaacompanhamento cea
                        inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
                        inner join pes.pescontrato con on con.concodigo = cea.concodigo
                        inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
                    where ceaano in ($ano)
                    and con.entcodigo = $entcodigo
                    and ccdtipoconfig = 'CA'
                    and ccototaliza = 'S'
                    group by lcocodigo

                    union all

                    -- Material de Consumo
                    select
                        can.canmes as lcocodigo,
                        sum(can.canvalor) as total
                    from pes.pescelulaacompnatdespesa can
                        inner join pes.pescontratonaturezadespesa cnd on cnd.cndcodigo = can.cndcodigo
                    where can.canano in ($ano)
                    and can.cantipovalor = 'FN'
                    and cnd.tidcodigo = " . K_DESPESA_MATERIAL_CONSUMO . "
                    and entcodigo = '$entcodigo'
                    group by can.canmes
                ) as executado
                group by lcocodigo
                order by lcocodigo";
    }

    $totais = $db->carregar($sql);
    $aExecutado = array();
    if($totais){
        foreach ($totais as $total) {
            $aExecutado[$total['lcocodigo']] = $total['total'];
        }
    }
    return $aExecutado;
}

?>
