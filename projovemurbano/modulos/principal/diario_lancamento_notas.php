<?php

require_once APPRAIZ . 'www/projovemurbano/_funcoes.php';

$perfis = pegaPerfilGeral();

/**
 * Gera um eleemento html - select
 * 
 * @param int $caeId
 * @return string 
 */
function geraCombo( $caeId, $caestatus, $difId, $esdId, $selected = null ){
    $combo = '';
 
    if( ($esdId == WF_ESTADO_DIARIO_ABERTO) )
    {
        $combo = '<select name="trabalho['. $difId .']['. $caeId .']" id="trabalho_'. $difId .'_'. $caeId .'"> ' ;
        if(!is_null($selected) && $selected == 't')
        {
            $combo .= '<option value="t" selected="selected">Sim</option> ' ;
        }else{
            $combo .= '<option value="t" >Sim</option> ' ;
        }

        if(!is_null($selected) && $selected == 'f')
        {
            $combo .= '<option value="f" selected="selected">Não</option> ' ;
        }else{
            $combo .= '<option value="f" >Não</option> ' ;
        }

        $combo .= '</select> ' ;
    }
    else 
    {
        if( $selected == 't' )
        {
            $combo = 'Sim';
        }
        else
        {
            $combo = 'Não';
        }
    }
    
    return $combo;
}


$perId  = (int) $_REQUEST['perid'];
$turId  = (int) $_REQUEST['turid'];

$sqlDadosDiario = "SELECT dif.difid, coc.cocnome, coc.cocqtdhoras, tur.turdesc
                        , unf.unfdesc, cic.cicdesc, nuc.nucid, ent.entnome
                        , TO_CHAR( per.perdtinicio, 'DD/MM/YYYY') as perdtinicio
                        , TO_CHAR( per.perdtfim, 'DD/MM/YYYY') as perdtfim                        
                        , ende.endlog, ende.endbai, ende.estuf, ende.endnum
                        , pmun.mundsc, doc.esdid, dia.diaid
                    FROM projovemurbano.diariofrequencia dif
                    INNER JOIN projovemurbano.diario dia
                        ON dif.diaid = dia.diaid
                    INNER JOIN workflow.documento doc
                        ON dia.docid = doc.docid                    
                    INNER JOIN projovemurbano.gradecurricular grd
                        ON dif.grdid = grd.grdid
                    INNER JOIN projovemurbano.componentecurricular coc
                        ON grd.cocid = coc.cocid
                    INNER JOIN projovemurbano.periodocurso per
                        ON dia.perid = per.perid
                    INNER JOIN projovemurbano.unidadeformativa unf
                        ON per.unfid = unf.unfid
                    INNER JOIN projovemurbano.ciclocurso cic
                        ON unf.cicid = cic.cicid
                    INNER JOIN projovemurbano.turma tur
                        ON dia.turid = tur.turid
                    INNER JOIN projovemurbano.nucleo nuc
                        ON tur.nucid = nuc.nucid
                    LEFT JOIN projovemurbano.nucleoescola nes
                        ON nuc.nucid = nes.nucid
                        AND nes.nuetipo = 'S' 
                    LEFT JOIN entidade.entidade ent
                        ON nes.entid = ent.entid
                    LEFT JOIN entidade.endereco ende
                        ON ent.entid = ende.entid
                    LEFT OUTER JOIN municipio pmun 
                        ON ende.muncod = pmun.muncod 
                    WHERE coc.cocdisciplina   = 'T'
                    AND dia.turid           = {$turId}
                    LIMIT 1";

$sqAlunoTurma = "SELECT DISTINCT cae.caeid, cae.caenome--, dia.diaid
						,cae.caestatus
                        ,npc.notaciclo1                          
                        ,npc.notaciclo2
                        ,npc.notaciclo3
                        ,cae.caecpf
                        --, (npc.notaciclo1  + npc.notaciclo2 + npc.notaciclo3) as notatotal
                FROM projovemurbano.diario dia                
                INNER JOIN projovemurbano.turma tur
                    ON dia.turid = tur.turid
                INNER JOIN projovemurbano.cadastroestudante cae
                    ON tur.turid = cae.turid
                LEFT JOIN projovemurbano.notasporciclo npc ON npc.caeid = cae.caeid AND  npc.turid = dia.turid AND npc.npc_status != 'I'
                WHERE dia.turid  = {$turId}
                --AND cae.caestatus = 'A' 
                ORDER BY cae.caenome";
// ver($sqAlunoTurma,d);               
$sqAlunoTurmaTransferidos = "SELECT DISTINCT cae.caeid, cae.caenome
                                ,npc.notaciclo1                          
                                ,npc.notaciclo2
                                ,npc.notaciclo3
                                ,cae.caecpf 
                            FROM projovemurbano.diario dia                
                            INNER JOIN projovemurbano.turma tur
                                ON dia.turid = tur.turid
                            INNER JOIN projovemurbano.transferencia tra 
                                INNER JOIN projovemurbano.cadastroestudante cae ON tra.cad_caeid = cae.caeid
                                INNER JOIN projovemurbano.historico_transferencia hst ON hst.traid = tra.traid
                            ON tra.turid_origem = tur.turid
                            LEFT JOIN projovemurbano.notasporciclo npc ON npc.caeid = cae.caeid AND npc.npc_status = 'I'
                            WHERE dia.turid  =  {$turId}
                            --AND cae.caestatus = 'A' 
                            AND hst.shtid_status = 3
                            ORDER BY cae.caenome";
//  ver($sqAlunoTurmaTransferidos,d);                                        
$sqlTrabalhosTurmaPeriodo = "SELECT dif.difid
                                FROM projovemurbano.diario dia
                                INNER JOIN projovemurbano.diariofrequencia dif
                                    ON dia.diaid = dif.diaid
                                INNER JOIN projovemurbano.gradecurricular grd
                                    ON dif.grdid = grd.grdid
                                INNER JOIN projovemurbano.componentecurricular coc
                                    ON grd.cocid = coc.cocid
                                WHERE dia.turid 	=  {$turId}
                                AND coc.cocdisciplina = 'T'";
//  ver($sqlTrabalhosTurmaPeriodo,d);                              
$sqlTrabalhosAlunos    = "SELECT cae.caeid, dia.diaid, dif.difid, coc.cocnome, coc.cocid
                                , frq.frqid, COALESCE(frq.frqtrabalho, 'f' ) AS frqtrabalho
                            FROM projovemurbano.diario dia
                            INNER JOIN projovemurbano.diariofrequencia dif
                                ON dif.diaid = dia.diaid
                            INNER JOIN projovemurbano.cadastroestudante cae
                                ON dia.turid = cae.turid
                            INNER JOIN projovemurbano.gradecurricular grd
                                ON dif.grdid = grd.grdid
                            INNER JOIN projovemurbano.componentecurricular coc
                                ON grd.cocid = coc.cocid
                            LEFT JOIN projovemurbano.frequenciaestudante frq
                                ON frq.difid  = dif.difid    
                                AND cae.caeid = frq.caeid
                            WHERE coc.cocdisciplina = 'T'
                            AND dia.turid = {$turId}
                            --AND cae.caestatus = 'A'
                            ORDER BY dif.difid, frq.caeid";
$arrAlunosTurma                 = $db->carregar($sqAlunoTurma);
$arrAlunosTurmaTransferidos     = $db->carregar($sqAlunoTurmaTransferidos);
$arrTrabalhosTurma              = $db->carregar($sqlTrabalhosTurmaPeriodo);
$dadosDiario                    = $db->pegaLinha($sqlDadosDiario);
//municipial
if($_SESSION['projovemurbano']['muncod'])
{
    $sql = "SELECT DISTINCT '1' as codigo, mu.mundescricao, mu.estuf
            , CASE
                WHEN pol.polid  IS NULL THEN 'Não Consta' 
                ELSE pol.polid || ''
              END as polnumero, mun.munid
            FROM projovemurbano.nucleo nuc
            INNER JOIN projovemurbano.nucleoescola nes 
                ON nes.nucid = nuc.nucid
            INNER JOIN entidade.entidade ent 
                ON ent.entid = nes.entid
            INNER JOIN projovemurbano.turma tur
                ON tur.nucid = nuc.nucid
            INNER JOIN projovemurbano.diario dia
                ON tur.turid = dia.turid
            INNER JOIN projovemurbano.diariofrequencia dif
                ON dia.diaid = dif.diaid
            LEFT JOIN projovemurbano.municipio mun 
                ON mun.munid = nuc.munid 
            LEFT JOIN territorios.municipio mu 
                ON mu.muncod = mun.muncod 
            LEFT JOIN projovemurbano.associamucipiopolo asm 
                ON asm.munid = mun.munid 
            LEFT JOIN projovemurbano.polo pol 
                ON pol.polid = asm.polid 
            LEFT JOIN projovemurbano.polomunicipio pm 
                ON pm.pmuid = pol.pmuid 
            WHERE mun.munstatus = 'A' 
            AND nuc.nucstatus   = 'A' 
            AND nes.nuetipo     = 'S'
            AND dia.turid       = {$turId}";
            
    $dado = $db->pegaLinha($sql);
    
}

//estadual
if($_SESSION['projovemurbano']['estuf'])
{
    $sql = "SELECT DISTINCT '2' as codigo, mu.estuf
                , CASE
                    WHEN pol.polid  IS NULL THEN 'Não Consta' 
                    ELSE pol.polid || ''
                  END as polnumero, mun.munid, mu.mundescricao
            FROM projovemurbano.nucleo nuc
            INNER JOIN projovemurbano.nucleoescola nes 
                ON nes.nucid = nuc.nucid
            INNER JOIN entidade.entidade ent 
                ON ent.entid = nes.entid
            INNER JOIN projovemurbano.municipio mun 
                ON mun.munid = nuc.munid 
            INNER JOIN territorios.municipio mu 
                ON mu.muncod = mun.muncod
            INNER JOIN territorios.estado est
                ON mu.estuf = est.estuf
            INNER JOIN projovemurbano.turma tur
                ON tur.nucid = nuc.nucid
            INNER JOIN projovemurbano.diario dia
                ON tur.turid = dia.turid
            INNER JOIN projovemurbano.diariofrequencia dif
                ON dia.diaid = dif.diaid
            LEFT JOIN projovemurbano.polomunicipio pm 
                ON pm.pmuid = mun.pmuid 
            LEFT JOIN projovemurbano.polo pol 
                 ON pm.pmuid = pol.pmuid 
            WHERE munstatus   = 'A' 
            AND nuc.nucstatus = 'A' 
            AND nes.nuetipo   = 'S'
            AND dia.turid     = {$turId}";
            
    $dado               = $db->pegaLinha($sql);
    $pmupossuipolo = $db->pegaUm("SELECT pmupossuipolo FROM projovemurbano.polomunicipio pmu WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
    
    if( $pmupossuipolo == 't' && $dado['munid']!= '' )
        $dado['polnumero']  = $db->pegaUm("SELECT polid FROM projovemurbano.associamucipiopolo WHERE munid='".$dado['munid']."'") . ' - '. $dado['mundescricao'] .' - '. $dado['estuf'];	
}

$sqlDadosInstituicao = "SELECT DISTINCT ee.entnome, ee.entcodent
                        FROM projovemurbano.usuarioresponsabilidade ur
                        inner join  projovemurbano.nucleoescola ne on ne.entid = ur.entid
                        inner join  entidade.entidade ee on ee.entid = ur.entid
                        where ur.usucpf = '{$_SESSION['usucpf']}'";
//ver($sqlDadosInstituicao,d);
$dadosInstituicao       = $db->pegaLinha($sqlDadosInstituicao);
$dadosTrabalhoAluno     = $db->carregar($sqlTrabalhosAlunos);
$arrTrabalhoAlunoTurma  = array();

if( is_array($dadosTrabalhoAluno) )
{
    foreach( $dadosTrabalhoAluno as $trabalhoAluno )
    {
        $arrTrabalhoAlunoTurma[$trabalhoAluno['caeid']][$trabalhoAluno['difid']] = $trabalhoAluno['frqtrabalho'];
    }
}

//PFL_DIRETOR_NUCLEO
//PFL_ADMINISTRADOR



//echo "<pre>";
//var_dump($_SESSION);
//exit;




if($db->testa_superuser()){
    $save = 'S';
} else {
    $sql = "select *
            from seguranca.perfilusuario
            where usucpf = '{$_SESSION['usucpf']}'
            and (pflcod = " . PFL_DIRETOR_NUCLEO . 
            " OR pflcod = " . PFL_ADMINISTRADOR . ")";
                
    $result = $db->pegaLinha($sql);

    if(!$result || date('Ymd') > '20131207'){
    	
/* Alterado dia 06/12/2013 a mando da Hellem. Tirar trava para alteração de nota por período indeterminado.*/
//         $save = 'N';
        $save = 'S';
    } else {
        $save = 'S';
    }
}
?>

<style>
    .textoCentro{
        text-align: center;
    }
    .colunaLabel {
        width: 200px;
    }
    .colunaPresenca{
        width: 150px;
    }
    .nomeAluno{
        width: 1500px;
    }
    
    #presenca tbody tr td{
        border: 1px solid black;
        border-top: none;
        border-right: none;
        padding: 5px;
        
    }
    #presenca thead tr td{
        border: 1px solid black;
        border-right: none;
    }
    
    #tableEducadores tbody tr td{
        border: 1px solid black;
        border-top: none;
        border-right: none;
        padding: 5px;
        
    }
    
    #tableEducadores thead tr td{
        border: 1px solid black;
        border-right: none;
    }
    
    
    .mes{
        font-size: 15px; 
        font-weight: bold;
    }
    
    .gray{
        background-color: #dcdcdc;
    }
</style>
<link rel="stylesheet" media="print" type="text/css" href="../projovemurbano/css/print_diario.css" />
<div>
    <table id="tableDiarioFrequencia" style="width: 100%;" border="0" >
        <tr>
            <td colspan="5">
                <!-- Cabeçalho -->
                <table id="cabecalho" border="0" style="width: 100%; border: 1px solid black;">
                    <tr>
                        <td class="textoCentro"><strong>RESULTADO DO PROCESSO AVALIATIVO DO PROJOVEM URBANO</strong></td>
                    </tr>
                </table>
            </td>
        </tr>
<!--        <tr class="exibir-info">
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%; background-color: #cccccc;">
            Placodigo
        </td>
        <td>
            <?php echo campo_texto('placodigo', 'S' , 'S', 'Descrição' , 25, '30', '', '', '', '', '', 'id="placodigo"') ?>
        </td>
    </tr>-->
        
        <!-- Informacoes -->
        <tbody>
            <tr>
                <td rowspan="4" style="width: 150px;"> <img  src="../imagens/projovemurbano_cinza.jpg" alt="PROJOVEM URBANO" title="PROJOVEMURBANO" /> </td>
                <td class="SubtituloDireita"  width="10%">Coordenação:</td>
                <td colspan="2"><?php echo montaTituloEstMun(); ?></td>
                <td></td>
                <td> </td>
            </tr>
            <tr>
                <td class="SubtituloDireita"  width="10%">Pólo:</td>
                <td colspan="2"><?php echo $dado['polnumero']; ?></td>
                <td></td>
                <td></td>
            </tr>
            <tr>
                <td class="SubtituloDireita"  width="10%">Núcleo:</td>
                <td colspan="2"><?php echo 'Escola: ', $dadosDiario['entnome'], ' - Núcleo: ', $dadosDiario['nucid']; ?></td>
                <td></td>
                <td></td>
            </tr>
            <tr>
                <td class="SubtituloDireita"  width="10%">Turma:</td>
                <td colspan="2"><?php echo $dadosDiario['turdesc']; ?></td>
            </tr>
        </tbody>
        
         <!-- Componente Curricular -->
         <tr>
             <td colspan="5">
                 <?php if($save == 'S'): ?>
                 <form name="frmTrabalho" id="frmTrabalho" method="post" action="">
                <?php endif ?>
                    <table style="width: 100%; " id="presenca" cellspacing="0" cellpadding="0" border="0">
                        <thead>
                            <tr>
                                <td rowspan="2" class="textoCentrogray gray" style="width: 200px; " >&nbsp;Nº</td>
                                <td rowspan="2" class="textoCentrogray gray" style="width: 200px;">&nbsp;CPF</td>
                                <td rowspan="2" class="textoCentrogray gray nomeAluno"> &nbsp;Estudante</td>
                                <td colspan="3" class="textoCentro" style="border-bottom: 0px solid black; width: 200px;">Pontuação</td>
                                <td rowspan="2" class="textoCentro gray" style="border-right: 1px solid black; width: 200px;">Pontuação Total</td>
                            </tr>
                            <tr>
                                <td class="textoCentro gray" style="width: 200px;">1º Ciclo ( UF I e II)</td>
                                <td class="textoCentro gray" style="width: 200px;">2º Ciclo ( UF III e IV)</td>
                                <td class="textoCentro gray" style="width: 200px;">3º Ciclo ( UF V e VI)</td>
                            </tr>
                        </thead>
                        <tbody>
                            <?php
                            if( $arrAlunosTurma == false ) {
                                echo '<td colspan="7" style="border-right: 1px solid black;">Não foram encontrados alunos na turma</td>';
                            }else {
                                $totalAlunos    = count($arrAlunosTurma);
                                $numero         = 0 ;
                                $totalTrabalho  = count($arrTrabalhosTurma);
                                for( $ct=0; $ct < $totalAlunos; $ct++)
                                {
                                    $notaTotal = $arrAlunosTurma[$ct]['notaciclo1'] + $arrAlunosTurma[$ct]['notaciclo2'] + $arrAlunosTurma[$ct]['notaciclo3'];
                                    $caestatus = $arrAlunosTurma[$ct]['caestatus'];
                                    $habil = '';
                                    if($caestatus!='A' || in_array( PFL_CONSULTA, $perfis ) ){
                                    	$habil = 'N';
                                    }
                                    $numero = $ct + 1;
                                    echo '<tr>';
                                    echo '<input type="hidden" id="name_' . $arrAlunosTurma[$ct]['caeid'] . '" value="' . $arrAlunosTurma[$ct]['caenome'] .  '" />';
                                    echo "<td>{$numero}</td>";
                                    echo "<td>{$arrAlunosTurma[$ct]['caecpf']}</td>";
                                    echo "<td class=\"nomeAluno\">{$arrAlunosTurma[$ct]['caenome']}</td>";

                                    echo '<td class="textoCentro" '.$disable.'>' . campo_texto("ciclo[{$arrAlunosTurma[$ct]['caeid']}][notaciclo1]", 'N',$habil, 'Descrição', 4, 6, '###,##', '', '', '', '', ' id="ciclo_1_' . $arrAlunosTurma[$ct]['caeid'] . '" ' , '', str_replace('.',',' , $arrAlunosTurma[$ct]['notaciclo1']) ) .'</td>';
                                    echo '<td class="textoCentro" '.$disable.'>' . campo_texto("ciclo[{$arrAlunosTurma[$ct]['caeid']}][notaciclo2]", 'N',$habil, 'Descrição', 4, 6, '###,##', '', '', '', '', ' id="ciclo_2_' . $arrAlunosTurma[$ct]['caeid'] . '" ' , '', str_replace('.',',' , $arrAlunosTurma[$ct]['notaciclo2']) ) .'</td>';
                                    echo '<td class="textoCentro" '.$disable.'>' . campo_texto("ciclo[{$arrAlunosTurma[$ct]['caeid']}][notaciclo3]", 'N',$habil, 'Descrição', 4, 6, '###,##', '', '', '', '', ' id="ciclo_3_' . $arrAlunosTurma[$ct]['caeid'] . '" ' , '', str_replace('.',',' , $arrAlunosTurma[$ct]['notaciclo3']) ) .'</td>';
                                    echo '<td style="border-right: 1px solid black;" id="container_notatotal_' . $arrAlunosTurma[$ct]['caeid'] .'">&nbsp; ' .  str_replace('.',',' , $notaTotal) . '</td>';

                                    echo '</tr>';
                                }
                            }
                            ?>
                        </tbody>
                    </table>
                     <input type="hidden" name="acao" id="acao" value="" />
                     <input type="hidden" name="diaid" id="diaid" value="<?php echo $dadosDiario['diaid'];?>" />
                     <input type="hidden" name="turid" id="turid" value="<?php echo $turId;?>" />
                     <?php if($save == 'S'): ?>
                 </form>
                 <?php endif ?>
             </td>
         </tr>
<!--         <tr>
             <td colspan="5" style="padding-top: 15px;">
                 <strong>Data de Emissão: <?php echo date("d/m/Y - H:i:s") ?></strong>
             </td>
         </tr>-->
         <tr>
            <td colspan="5">
                <!-- Cabeçalho -->
                <table id="cabecalho" border="0" style="width: 100%; border: 1px solid black;">
                    <tr>
                        <td><strong>Data de Emissão: <?php echo date("d/m/Y - H:i:s") ?></strong></td>
                    </tr>
                </table>
            </td>
        </tr>
         <tr>
            <td colspan="5">
                <!-- Cabeçalho -->
                <table id="cabecalho" border="0" style="width: 100%; border: 1px solid black;">
                    <tr>
                        <td><strong>Alunos que foram transferidos: </strong></td>
                    </tr>
                </table>
            </td>
        </tr>
         <!-- Componente Curricular -->
         <tr>
             <td colspan="5">
                 <form name="frmTrabalho" id="frmTrabalho" method="post" action="">
                    <table style="width: 100%; " id="presenca" cellspacing="0" cellpadding="0" border="0">
                        <thead>
                            <tr>
                                <td rowspan="2" class="textoCentrogray gray" style="width: 200px; " >&nbsp;Nº</td>
                                <td rowspan="2" class="textoCentrogray gray" style="width: 200px;">&nbsp;CPF</td>
                                <td rowspan="2" class="textoCentrogray gray nomeAluno"> &nbsp;Estudante</td>
                                <td colspan="3" class="textoCentro" style="border-bottom: 0px solid black; width: 200px;">Pontuação</td>
                                <td rowspan="2" class="textoCentro gray" style="border-right: 1px solid black; width: 200px;">Pontuação Total</td>
                            </tr>
                            <tr>
                                <td class="textoCentro gray" style="width: 200px;">1ºCiclo ( UF I e II)</td>
                                <td class="textoCentro gray" style="width: 200px;">2ºCiclo ( UF III e IV)</td>
                                <td class="textoCentro gray" style="width: 200px;">3ºCiclo ( UF V e VI)</td>
                            </tr>
                        </thead>
                        <tbody>
                            <?php
                            if( $arrAlunosTurmaTransferidos == false ) {
                                echo '<td colspan="7" style="border-right: 1px solid black;">Não foram encontrados alunos na turma</td>';
                            }else {
                                $totalAlunos    = count($arrAlunosTurmaTransferidos);
                                $numero         = 0 ;
                                $totalTrabalho  = count($arrTrabalhosTurma);
                                for( $ct=0; $ct < $totalAlunos; $ct++)
                                {
                                    $notaTotal = $arrAlunosTurmaTransferidos[$ct]['notaciclo1'] + $arrAlunosTurmaTransferidos[$ct]['notaciclo2'] + $arrAlunosTurmaTransferidos[$ct]['notaciclo3'];
                                    
                                    $numero = $ct + 1;
                                    echo '<tr>';
                                    echo '<input type="hidden" id="name_' . $arrAlunosTurmaTransferidos[$ct]['caeid'] . '" value="' . $arrAlunosTurmaTransferidos[$ct]['caenome'] .  '" />';
                                    echo "<td>{$numero}</td>";
                                    echo "<td>{$arrAlunosTurmaTransferidos[$ct]['caecpf']}</td>";
                                    echo "<td class=\"nomeAluno\">{$arrAlunosTurmaTransferidos[$ct]['caenome']}</td>";

                                    echo '<td class="textoCentro">' . campo_texto("ciclo[{$arrAlunosTurmaTransferidos[$ct]['caeid']}][notaciclo1]", 'N' , 'N', 'Descrição' , 4, '6', '###,##', '', '', '', '', 'id="ciclo_1_' . $arrAlunosTurmaTransferidos[$ct]['caeid'] . '"' , str_replace('.',',' , $arrAlunosTurmaTransferidos[$ct]['notaciclo1'])) .'</td>';
                                    echo '<td class="textoCentro">' . campo_texto("ciclo[{$arrAlunosTurmaTransferidos[$ct]['caeid']}][notaciclo2]", 'N' , 'N', 'Descrição' , 4, '6', '###,##', '', '', '', '', 'id="ciclo_2_' . $arrAlunosTurmaTransferidos[$ct]['caeid'] . '"' , str_replace('.',',' , $arrAlunosTurmaTransferidos[$ct]['notaciclo2'])) .'</td>';
                                    echo '<td class="textoCentro">' . campo_texto("ciclo[{$arrAlunosTurmaTransferidos[$ct]['caeid']}][notaciclo3]", 'N' , 'N', 'Descrição' , 4, '6', '###,##', '', '', '', '', 'id="ciclo_3_' . $arrAlunosTurmaTransferidos[$ct]['caeid'] . '"' , str_replace('.',',' , $arrAlunosTurmaTransferidos[$ct]['notaciclo3'])) .'</td>';
                                    echo '<td style="border-right: 1px solid black;" id="container_notatotal_' . $arrAlunosTurma[$ct]['caeid'] .'">&nbsp; ' .  str_replace('.',',' , $notaTotal) . '</td>';

                                    echo '</tr>';
                                }
                            }
                            ?>
                        </tbody>
                    </table>
                     <input type="hidden" name="acao" id="acao" value="" />
                     <input type="hidden" name="diaid" id="diaid" value="<?php echo $dadosDiario['diaid'];?>" />
                     <input type="hidden" name="turid" id="turid" value="<?php echo $turId;?>" />
                 </form>
             </td>
         </tr>
    </table>
    <div id="btnImpressao" style="width: 100%; text-align: center;">
        <?  if(/* $dadosDiario['esdid'] == WF_ESTADO_DIARIO_ABERTO && */!in_array( PFL_CONSULTA, $perfis ) ) { ?>
        <?php if($save == 'S'): ?>
            <input type="button" name="btnSalvarTrabalho" id="btnSalvarTrabalho" value="Salvar" />
        <?php endif; ?>
            <input type="button" name="btnFecharTrabalho" id="btnImprimir" value="Imprimir" />
        <? } ?>
    </div>
</div>

<script lang="javascript">
    
    $('#btnSalvarTrabalho').click(function(){
        $('#acao').val('salvarLancamentoNotas');
        params = $('#frmTrabalho').serialize();
        
        $.post( 'geral/ajax.php', params, function(response){
            alert(response);
//            $('#container-diario').html( response );
//             $('#btnVisualizarDiario').attr('disabled', false);
        }, 'html' );
        
    });
    
    
    $('#btnImprimir').click(function(){
        window.print();
    });
    
    function str_replace(search, replace, subject, count) {
    // http://kevin.vanzonneveld.net
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   improved by: Gabriel Paderni
    // +   improved by: Philip Peterson
    // +   improved by: Simon Willison (http://simonwillison.net)
    // +    revised by: Jonas Raoni Soares Silva (http://www.jsfromhell.com)
    // +   bugfixed by: Anton Ongson
    // +      input by: Onno Marsman
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +    tweaked by: Onno Marsman
    // +      input by: Brett Zamir (http://brett-zamir.me)
    // +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   input by: Oleg Eremeev
    // +   improved by: Brett Zamir (http://brett-zamir.me)
    // +   bugfixed by: Oleg Eremeev
    // %          note 1: The count parameter must be passed as a string in order
    // %          note 1:  to find a global variable in which the result will be given
    // *     example 1: str_replace(' ', '.', 'Kevin van Zonneveld');
    // *     returns 1: 'Kevin.van.Zonneveld'
    // *     example 2: str_replace(['{name}', 'l'], ['hello', 'm'], '{name}, lars');
    // *     returns 2: 'hemmo, mars'
    var i = 0,
            j = 0,
            temp = '',
            repl = '',
            sl = 0,
            fl = 0,
            f = [].concat(search),
            r = [].concat(replace),
            s = subject,
            ra = Object.prototype.toString.call(r) === '[object Array]',
            sa = Object.prototype.toString.call(s) === '[object Array]';
    s = [].concat(s);
    if (count) {
        this.window[count] = 0;
    }

    for (i = 0, sl = s.length; i < sl; i++) {
        if (s[i] === '') {
            continue;
        }
        for (j = 0, fl = f.length; j < fl; j++) {
            temp = s[i] + '';
            repl = ra ? (r[j] !== undefined ? r[j] : '') : r[0];
            s[i] = (temp).split(f[j]).join(repl);
            if (count && s[i] !== temp) {
                this.window[count] += (temp.length - s[i].length) / f[j].length;
            }
        }
    }
    return sa ? s : s[0];
}
    
    function MouseBlur(input)
    {
        var input = $(input);
//         if(input.val() != ''){
            arrInput = input.attr('id').split('_');
            if( arrInput[1] == 1 && str_replace(',' , '' , input.val()) > 73000 ){
                input.val('');
                input.focus();
                alert('O valor "1ºCiclo ( UF I e II)" do(a) aluno(a) "' + $('#name_' + arrInput[2]).val() + '" não pode ser maior que ' + 730  + '!');
            } else if (arrInput[1] == 2 &&  str_replace(',' , '' , input.val()) > 66000 ) {
                input.val('');
                input.focus();
                alert('O valor do "2ºCiclo ( UF III e IV)" do(a) aluno(a) "' + $('#name_' + arrInput[2]).val() + '" não pode ser maior que ' + 660  + '!');
            } else if(arrInput[1] == 3 &&  str_replace(',' , '' , input.val()) > 81000 ) {
                input.val('');
                input.focus();
                alert('O valor do "3ºCiclo ( UF V e VI)" do(a) aluno(a) "' + $('#name_' + arrInput[2]).val() + '" não pode ser maior que ' + 810  + '!');
            } else {
                
                var nota1 = str_replace(',' , '.' , $('#ciclo_1_' + arrInput[2]).val());
                var nota2 = str_replace(',' , '.' , $('#ciclo_2_' + arrInput[2]).val());
                var nota3 = str_replace(',' , '.' , $('#ciclo_3_' + arrInput[2]).val());

				if(nota1 == '' || nota1 == null){
					nota1 = 0;
				}
				if(nota2 == '' || nota2 == null){
					nota2 = 0; 
				}
                if(nota3 == '' || nota3 == null){
					nota3 = 0;
                }
                var notaTotal = 0;
                if(nota1)
                    notaTotal += parseFloat(nota1);
                if(nota2)
                    notaTotal += parseFloat(nota2);
                if(nota3)
                    notaTotal += parseFloat(nota3);
                
                $('#container_notatotal_' + arrInput[2]).hide().html( '&nbsp;&nbsp;' + str_replace('.' , ',' , notaTotal)).fadeIn();
//                $('#container_notatotal_' + arrInput[2]).fadeOut().hide().html( '&nbsp;&nbsp;' + str_replace('.' , ',' , notaTotal)).fadeIn();
                
//                alert(str_replace(',' , '.' , $('#ciclo_1_' + arrInput[2]).val()));
//                alert(parseFloat(nota2));
//                alert(notaTotal);
            }
//            alert(str_replace(',' , '' , input.val()));
//            alert(input.val());
//            alert(input.attr('id'));
//            alert(input.attr('id').split('_'));
//            alert(arrInput[1]);
//            console.info(arrInput);
        
//        if(input.val() > 30){
//            
//        }
            
//         } 
        input.attr('class', 'normal');
    }
</script>
    
