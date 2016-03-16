<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if($_REQUEST['download'] == 'S'){
    $file = new FilesSimec();
    $arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';

$where[] = $_POST['esfera'] == 'E' ? ' iu.itrid = 1 ' : ' iu.itrid = 2 ';
if($_POST['estuf']){
    $where[] = $_POST['esfera'] == 'E' ? " iu.estuf = '{$_POST['estuf']}' " : " mun.estuf = '{$_POST['estuf']}' ";
}
if($_POST['mundescricao']){
    $where[] = " mun.mundescricao ilike '%{$_POST['mundescricao']}%' ";
}
if($_POST['tpmid']){
	$left_tipomunicipio = "inner join territorios.muntipomunicipio mtm on mtm.muncod = mun.muncod and mtm.tpmid=".$_POST['tpmid'];
}
if($_POST['bncperg1']){
    $where[] = " bncperg1 = '{$_POST['bncperg1']}' ";
}
if($_POST['bncperg2']){
    $where[] = " bncperg2 = '{$_POST['bncperg2']}' ";
}
if($_POST['bncperg3']){
    $where[] = " bncperg3 = '{$_POST['bncperg3']}' ";
}

$sql = "select  bn.bncid, bna.bnaid, bna.arqid, arq.arqnome, arq.arqdescricao, case when iu.itrid = 1 then 'Estadual' else 'Municipal' end Esfera,
                case when iu.itrid = 1 then iu.estuf else mun.estuf || ' - ' || mun.mundescricao end descricao,
                case
                        when bncperg1 = 'a' then 'a - Sim'
                        when bncperg1 = 'b' then 'b - Não'
                        when bncperg1 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg1 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg1
                end as bncperg1,
                case
                        when bncperg2 = 'a' then 'a - Sim'
                        when bncperg2 = 'b' then 'b - Não'
                        when bncperg2 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg2 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg2
                end as bncperg2,
                case
                        when bncperg3 = 'a' then 'a - Sim'
                        when bncperg3 = 'b' then 'b - Não'
                        when bncperg3 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg3 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg3
                end as bncperg3,
                case
                        when bncperg4 = 'a' then 'a - Sim'
                        when bncperg4 = 'b' then 'b - Não'
                        when bncperg4 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg4 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg4
                end as bncperg4,
                case
                        when bncperg5 = 'a' then 'a - Sim'
                        when bncperg5 = 'b' then 'b - Não'
                        when bncperg5 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg5 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg5
                end as bncperg5
                from par.basenacionalcomum bn
                    inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                    left join territorios.municipio mun on mun.muncod = iu.muncod 
					{$left_tipomunicipio}
                    left  join par.basenacionalcomumarquivo bna on bna.bncid = bn.bncid
                    left  join public.arquivo arq on arq.arqid = bna.arqid
                where " . implode(' AND ', $where) . "
                order by esfera, descricao";

$resultado = $db->carregar($sql);
$resultado = $resultado ? $resultado : array();

$dados = array();
foreach ($resultado as $dado) {
    $dados[$dado['bncid']][] = $dado;
}

extract($_POST);

$perguntas = array(
    array('codigo'=>'a', 'descricao'=>'a - Sim'),
    array('codigo'=>'b', 'descricao'=>'b - Não'),
    array('codigo'=>'c', 'descricao'=>'c - Não, mas está em elaboração'),
    array('codigo'=>'d', 'descricao'=>'d - Não possuo informações sobre o assunto'),
);

?>

<form method="post" name="formulario" id="formulario">
    <table id="pesquisas" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tbody>
        <tr>
            <td class="SubTituloDireita">Esfera:</td>
            <td>
                <input type="radio" name="esfera" value="E" <?php echo $_POST['esfera'] == 'E' ? 'checked="checked"' : ''; ?> />Estadual
                <input type="radio" name="esfera" value="M" style="margin-left: 10px;" <?php echo $_POST['esfera'] != 'E' ? 'checked="checked"' : ''; ?> />Municipal
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" >Estado:</td>
            <td colspan="3">
                <?php
                $estuf = $_POST['estuf'];
                $sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
                $db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" >Município:</td>
            <td colspan="3">
                <?php
                $filtro = simec_htmlentities( $_POST['municipio'] );
                $municipio = $filtro;
                echo campo_texto( 'mundescricao', 'N', 'S', '', 50, 200, '', '');
                ?> 
                <input type="checkbox" name="tpmid" value="1" <?=(($_POST['tpmid']=='1')?'checked':'') ?> > Grandes Cidades e Capitais
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" >Pergunta 1:</td>
            <td colspan="3">
                <?php
                $db->monta_combo( "bncperg1", $perguntas, 'S', 'Todas', '', '' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" >Pergunta 2:</td>
            <td colspan="3">
                <?php
                $db->monta_combo( "bncperg2", $perguntas, 'S', 'Todas', '', '' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" >Pergunta 3:</td>
            <td colspan="3">
                <?php
                $db->monta_combo( "bncperg3", $perguntas, 'S', 'Todas', '', '' );
                ?>
            </td>
        </tr>
        <tr>
            <td colspan="2" align="center">
                <input type="button" name="enviar" id="enviar" value="Gerar Relatório"/>
            </td>
        </tr>
    </table>
</form>

<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
    <thead>
    <tr align="center">
        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
            <strong>Arquivos</strong>
        </td>
        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
            <strong>Esfera</strong>
        </td>
        <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
            <strong>Descrição</strong>
        </td>
        <td width="15%" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" title="O Sistema de Ensino sob sua responsabilidade possui orientações curriculares, ou matriz curricular, ou outra modalidade de documento referente à organização do currículo na EDUCAÇÃO INFANTIL elaboradas a partir de dezembro de 2009?">
            <strong>EDUCAÇÃO INFANTIL</strong>
        </td>
        <td width="15%" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" title="O Sistema de Ensino sob sua responsabilidade possui orientações curriculares, ou matriz curricular, ou outra modalidade de documento referente à organização do currículo no ENSINO FUNDAMENTAL - ANOS INICIAIS elaboradas a partir de julho de 2010?">
            <strong>ENSINO FUNDAMENTAL - ANOS INICIAIS</strong>
        </td>
        <td width="15%" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" title="O Sistema de Ensino sob sua responsabilidade possui orientações curriculares, ou matriz curricular, ou outra modalidade de documento referente à organização do currículo no ENSINO FUNDAMENTAL - ANOS FINAIS elaboradas a partir de julho de 2010?">
            <strong>ENSINO FUNDAMENTAL - ANOS FINAIS</strong>
        </td>
        <td width="15%" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" title="O Sistema de Ensino sob sua responsabilidade possui orientações curriculares, ou matriz curricular, ou outra modalidade de documento referente à organização do currículo no ENSINO MÉDIO elaboradas a partir de julho de 2010?">
            <strong>ENSINO MÉDIO</strong>
        </td>
        <td width="15%" valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" title="O Sistema de Ensino sob sua responsabilidade possui orientações curriculares, ou matriz curricular, ou outra modalidade de documento referente à organização do currículo na EDUCAÇÃO DE JOVENS E ADULTOS elaboradas a partir de julho de 2010?">
            <strong>EDUCAÇÃO DE JOVENS E ADULTOS</strong>
        </td>
    </tr>
    </thead>
    <tbody>
    <?php
        $count = 0;
        foreach ((array)$dados as $aArquivos) {
            $download = '';
            $arquivos = array();
            $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
            $count++;
            foreach ($aArquivos as $dado) {
                if($dado['arqid']){
                    $download .= '<a title="' . $dado['arqnome'] . '" style="cursor: pointer; color: blue;" onclick="window.location=\'?modulo=relatorio/relatorioBaseNacionalComum&acao=A&download=S&arqid=' . $dado['arqid'] . '&tipo=D\'"> <span class="glyphicon glyphicon-arrow-down"> </span> </a>';
                }
            }
            ?>
            <tr <?php echo $complemento; ?>>
                <td>
                    <?php echo $download; ?>
                </td>
                <td><?php echo $dado['esfera']; ?></td>
                <td><?php echo $dado['descricao']; ?></td>
                <td><?php echo $dado['bncperg1']; ?></td>
                <td><?php echo $dado['bncperg2']; ?></td>
                <td><?php echo $dado['bncperg4']; ?></td>
                <td><?php echo $dado['bncperg3']; ?></td>
                <td><?php echo $dado['bncperg5']; ?></td>
            </tr>
        <?php } ?>
    </tbody>
    <thead>
        <tr onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
            <td colspan="8">Total de registros: <?php echo count($dados); ?></td>
        </tr>
    </thead>
</table>

<script type="text/javascript">
    $1_11(function(){
        $1_11('#enviar').click(function(){
            $1_11('#formulario').submit();
        });
    });        
</script>