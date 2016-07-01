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
    if($caestatus!='A'){
    	$disable = 'disabled="disabled"';
    }
    if( $esdId == WF_ESTADO_DIARIO_ABERTO )
    {
        $combo = '<select '.$disable.' name="trabalho['. $difId .']['. $caeId .']" id="trabalho_'. $difId .'_'. $caeId .'"> ' ;
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

function geraCombo2( $caeid2, $difId, $esdId, $selected = null ){
    $combo = '';
//     if( $esdId == WF_ESTADO_DIARIO_ABERTO )
//     {
//         $combo = '<select name="trabalho['. $difId .']['. $caeid2 .']" id="trabalho_'. $difId .'_'. $caeid2 .'"> ' ;
//         if(!is_null($selected) && $selected == 't')
//         {
//             $combo .= '<option value="t" selected="selected">Sim</option> ' ;
//         }else{
//             $combo .= '<option value="t" >Sim</option> ' ;
//         }

//         if(!is_null($selected) && $selected == 'f')
//         {
//             $combo .= '<option value="f" selected="selected">Não</option> ' ;
//         }else{
//             $combo .= '<option value="f" >Não</option> ' ;
//         }

//         $combo .= '</select> ' ;
//     }
//     else 
//     {
        if( $selected == 't' )
        {
            $combo = 'Sim';
        }
        else
        {
            $combo = 'Não';
        }
//     }
    
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
                    WHERE per.perid         = {$perId}
                    AND coc.cocdisciplina   = 'T'
                    AND dia.turid           = {$turId}
                    LIMIT 1";

// $sqAlunoTurma = "SELECT cae.caeid, cae.caenome, dia.diaid, cae.caestatus
//                 FROM projovemurbano.diario dia                
//                 INNER JOIN projovemurbano.turma tur
//                     ON dia.turid = tur.turid
//                 INNER JOIN projovemurbano.cadastroestudante cae
//                     ON tur.turid = cae.turid
//                 WHERE dia.perid = {$perId}
//                 AND dia.turid  = {$turId}
//                 --AND cae.caestatus = 'A' 
//                 ORDER BY cae.caenome";

$sqAlunoTurma = "SELECT distinct cae.caeid, cae.caenome, dia.diaid, cae.caestatus
					FROM projovemurbano.diario dia
					INNER JOIN projovemurbano.diariofrequencia dif ON dif.diaid = dia.diaid  
					INNER JOIN projovemurbano.frequenciaestudante frq ON frq.difid = dif.difid           
					INNER JOIN projovemurbano.turma tur
					    ON dia.turid = tur.turid
					INNER JOIN projovemurbano.cadastroestudante cae
					    ON tur.turid = cae.turid AND cae.caeid = frq.caeid
					WHERE dia.perid = {$perId}
					AND dia.turid  = {$turId}
					--AND cae.caestatus = 'A' 
					ORDER BY cae.caenome";
$sqAlunoTurmaTransferido = "SELECT DISTINCT cae.caeid, cae.caenome, dia.diaid
							FROM
								projovemurbano.transferencia tra 
							INNER JOIN projovemurbano.historico_transferencia htr ON htr.traid = tra.traid AND htr.shtid_status = '3'
							INNER JOIN projovemurbano.diario dia ON tra.turid_origem = dia.turid
							INNER JOIN projovemurbano.cadastroestudante cae ON cae.caeid = tra.cad_caeid
							WHERE dia.perid = {$perId}
							--AND   cae.caestatus = 'A' 
							AND   tra.turid_origem = {$turId}
							ORDER BY cae.caenome";
      
$sqlTrabalhosTurmaPeriodo = "SELECT dif.difid
                                FROM projovemurbano.diario dia
                                INNER JOIN projovemurbano.diariofrequencia dif
                                    ON dia.diaid = dif.diaid
                                INNER JOIN projovemurbano.gradecurricular grd
                                    ON dif.grdid = grd.grdid
                                INNER JOIN projovemurbano.componentecurricular coc
                                    ON grd.cocid = coc.cocid
                                WHERE dia.perid = {$perId}
                                AND dia.turid 	=  {$turId}
                                AND coc.cocdisciplina = 'T'";
                                
$sqlTrabalhosAlunostransferidos="SELECT tra.cad_caeid, dia.diaid, dif.difid, coc.cocnome, coc.cocid, frq.frqid, COALESCE(frq.frqtrabalho, 'f' ) AS frqtrabalho                                 
                                      FROM projovemurbano.diario dia
                                      INNER JOIN projovemurbano.diariofrequencia dif ON dif.diaid = dia.diaid    
                                      INNER JOIN projovemurbano.transferencia tra ON tra.turid_origem  = dia.turid AND tra.turid_origem = {$turId}
                                      LEFT JOIN projovemurbano.historico_transferencia htr ON htr.traid = tra.traid AND htr.shtid_status = '3'
                                      INNER JOIN projovemurbano.gradecurricular grd ON dif.grdid = grd.grdid
                                      INNER JOIN projovemurbano.componentecurricular coc ON grd.cocid = coc.cocid
                                      LEFT JOIN projovemurbano.frequenciaestudante frq ON frq.difid  = dif.difid AND tra.cad_caeid = frq.caeid
                                      WHERE coc.cocdisciplina = 'T'
                                      AND dia.turid = {$turId}
                                      AND dia.perid = {$perId} 
                                      ORDER BY dif.difid, frq.caeid";  
                                      
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
                            AND dia.perid = {$perId}
                            --AND cae.caestatus = 'A'
                            ORDER BY dif.difid, frq.caeid";
                     
$arrsqAlunoTurmaTransferido     = $db->carregar($sqAlunoTurmaTransferido);
$arrAlunosTurma                 = $db->carregar($sqAlunoTurma);
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
            AND dia.perid       = {$perId}
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
            AND dia.perid     = {$perId}
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

$dadosInstituicao                = $db->pegaLinha($sqlDadosInstituicao);
$dadosTrabalhoAluno              = $db->carregar($sqlTrabalhosAlunos);
$dadosTrabalhosAlunostransferidos = $db->carregar($sqlTrabalhosAlunostransferidos);
$arrTrabalhoAlunoTurma  = array();

if( is_array($dadosTrabalhoAluno) )
{
    foreach( $dadosTrabalhoAluno as $trabalhoAluno )
    {
        $arrTrabalhoAlunoTurma[$trabalhoAluno['caeid']][$trabalhoAluno['difid']] = $trabalhoAluno['frqtrabalho'];
    }
}

if( is_array($dadosTrabalhosAlunostransferidos) )
{
    foreach( $dadosTrabalhosAlunostransferidos as $trabalhoAlunotransferido )
    {
        $arrTrabalhoAlunoTurmatransferido[$trabalhoAlunotransferido['cad_caeid']][$trabalhoAlunotransferido['difid']] = $trabalhoAlunotransferido['frqtrabalho'];
//     ver($arrTrabalhoAlunoTurmatransferido);   
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
</style>
<link rel="stylesheet" media="print" type="text/css" href="../projovemurbano/css/print_diario.css" />
<div>
    <table id="tableDiarioFrequencia" style="width: 100%;" border="0" >
        <tr>
            <td colspan="5">
                <!-- Cabeçalho -->
                <table id="cabecalho" border="0" style="width: 100%; border: 1px solid black;">
                    <tr>
                        <td rowspan="3" style="width: 150px;"> <img  src="../imagens/projovemurbano_cinza.jpg" alt="PROJOVEM URBANO" title="PROJOVEMURBANO" /> </td>
                        <td class="textoCentro"> Ministério da Educação </td>
                    </tr>
                    <tr>
                        <td class="textoCentro"> Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão </td>
                    </tr>
                    <tr>
                        <td class="textoCentro"><strong>DIÁRIO DE ENTREGA DE TRABALHOS</strong></td>
                    </tr>
                </table>
            </td>
        </tr>
        
        
        <!-- Informacoes -->
        <tbody>
            <tr>
                <td class="colunaLabel">Coordenação Municipal:</td>
                <td colspan="2"><?php echo montaTituloEstMun(); ?></td>
                <td></td>
                <td> </td>
            </tr>
            <tr>
                <td class="colunaLabel">Pólo:</td>
                <td colspan="2"><?php echo $dado['polnumero']; ?></td>
                <td></td>
                <td></td>
            </tr>
            <tr>
                <td class="colunaLabel">Núcleo:</td>
                <td colspan="2"><?php echo 'Escola: ', $dadosInstituicao['entnome'], ' - Núcleo: ', $dadosDiario['nucid']; ?></td>
                <td></td>
                <td></td>
            </tr>
            <tr>
                <td class="colunaLabel">Endereço:</td>
                <td colspan="2"><?php echo $dadosDiario['endlog'], ' ', $dadosDiario['endnum'], ' - ', $dadosDiario['endbai'], ' - ', $dadosDiario['mundsc'], '/', $dadosDiario['estuf'] ; ?></td>
                <td></td>
                <td></td>
            </tr>
            <tr>
                <td class="colunaLabel">Turma:</td>
                <td><?php echo $dadosDiario['turdesc']; ?></td>
                <td class="mes" colspan="3">Mês: <?php echo $dadosDiario['perdtinicio'], ' a ', $dadosDiario['perdtfim']; ?></td>
            </tr>
            <tr>
                <td class="colunaLabel">Ciclo:</td>
                <td colspan="2"><?php echo $dadosDiario['cicdesc']; ?></td>
                <td></td>
                <td></td>
            </tr>
            <tr>
                <td class="colunaLabel">Unidade Formativa:</td>
                <td colspan="2"><?php echo $dadosDiario['unfdesc']; ?></td>
                <td></td>
                <td></td>
            </tr>
        </tbody>
        
         <!-- Componente Curricular -->
         <tr>
             <td colspan="5">
                 <form name="frmTrabalho" id="frmTrabalho" method="post" action="">
                    <table style="width: 100%; " id="presenca" cellspacing="0" cellpadding="0" border="0">
                        <thead>
                            <tr>
                                <td class="textoCentro" style="width: 200px; " >Nº</td>
                                <td class="textoCentro" style="width: 200px;">Matrícula</td>
                                <td class="textoCentro nomeAluno">Estudante</td>
                                <td class="textoCentro" style="width: 200px;">Plano de Ação Comunitária</td>
                                <td class="textoCentro" style="width: 200px;">Projeto de Orientação Profissional</td>
                                <td class="textoCentro" style="width: 400px; border-right: 1px solid black;" colspan="2">Sínteses Integradoras</td>
                            </tr>

                        </thead>
                        <tbody>
                            <tr>
                                <td class="textoCentro" style="width: 200px; " >&nbsp;</td>
                                <td class="textoCentro" style="width: 200px;">&nbsp;</td>
                                <td class="textoCentro nomeAluno">&nbsp;</td>
                                <td class="textoCentro" style="width: 200px;">Trabalho 1</td>
                                <td class="textoCentro" style="width: 200px;">Trabalho 2</td>
                                <td class="textoCentro" style="width: 200px;">Trabalho 3</td>
                                <td class="textoCentro" style="width: 200px; border-right: 1px solid black;">Trabalho 4</td>
                            </tr>
                            <?php
                            if( $arrAlunosTurma == false ) {
                                echo '<td colspan="7" style="border-right: 1px solid black;">Não foram encontrados alunos na turma</td>';
                            }else {
                                $totalAlunos    = count($arrAlunosTurma);
                                $numero         = 0 ;
                                $totalTrabalho  = count($arrTrabalhosTurma);
                                for( $ct=0; $ct < $totalAlunos; $ct++)
                                {
                                    $numero = $ct + 1;
                                    echo '<tr>';
                                    echo "<td>{$numero}</td>";
                                    echo "<td>{$arrAlunosTurma[$ct]['caeid']}</td>";
                                    echo "<td class=\"nomeAluno\">{$arrAlunosTurma[$ct]['caenome']}</td>";

                                    $ctTrabalho     = 0;
                                    foreach( $arrTrabalhosTurma as $trabalhoTurma )
                                    {
                                        $difId  = $trabalhoTurma['difid'];
                                        $caeId  = $arrAlunosTurma[$ct]['caeid'];
                                        $caestatus = $arrAlunosTurma[$ct]['caestatus'];
                                        
                                        if( $ctTrabalho == ($totalTrabalho-1) )
                                            echo '<td style="border-right: 1px solid black;">', geraCombo( $caeId, $caestatus , $difId, $dadosDiario['esdid'], $arrTrabalhoAlunoTurma[$caeId][$difId] ), '</td>';
                                        else
                                            echo '<td>', geraCombo( $caeId, $caestatus, $difId, $dadosDiario['esdid'], $arrTrabalhoAlunoTurma[$caeId][$difId] ), '</td>';

                                        $ctTrabalho++;

                                    }

                                    echo '</tr>';
                                }
                            }
                            ?>
                        </tbody>
                    </table>
                     <input type="hidden" name="acao" id="acao" value="" />
                     <input type="hidden" name="diaid" id="diaid" value="<?php echo $dadosDiario['diaid'];?>" />
                 </form>
             </td>
         </tr>
         <tr>
            <td colspan="5" style="padding-top: 15px;">
                 Alunos que foram transferidos.
             </td>
         </tr>
          <tr>
             <td colspan="5">
                 <form name="frmTrabalho" id="frmTrabalho" method="post" action="">
                    <table style="width: 100%; " id="presenca" cellspacing="0" cellpadding="0" border="0">
                        <thead>
                            <tr>
                                <td class="textoCentro" style="width: 200px; " >Nº</td>
                                <td class="textoCentro" style="width: 200px;">Matrícula</td>
                                <td class="textoCentro nomeAluno">Estudante</td>
                                <td class="textoCentro" style="width: 200px;">Plano de Ação Comunitária</td>
                                <td class="textoCentro" style="width: 200px;">Projeto de Orientação Profissional</td>
                                <td class="textoCentro" style="width: 400px; border-right: 1px solid black;" colspan="2">Sínteses Integradoras</td>
                            </tr>

                        </thead>
                        <tbody>
                            <tr>
                                <td class="textoCentro" style="width: 200px; " >&nbsp;</td>
                                <td class="textoCentro" style="width: 200px;">&nbsp;</td>
                                <td class="textoCentro nomeAluno">&nbsp;</td>
                                <td class="textoCentro" style="width: 200px;">Trabalho 1</td>
                                <td class="textoCentro" style="width: 200px;">Trabalho 2</td>
                                <td class="textoCentro" style="width: 200px;">Trabalho 3</td>
                                <td class="textoCentro" style="width: 200px; border-right: 1px solid black;">Trabalho 4</td>
                            </tr>
                            <?php
                            if( $arrsqAlunoTurmaTransferido == false ) {
                                echo '<td colspan="7" style="border-right: 1px solid black;">Não foram encontrados alunos na turma</td>';
                            }else {
                                $totalAlunos    = count($arrsqAlunoTurmaTransferido);
                                $numero         = 0 ;
                                $totalTrabalho  = count($arrTrabalhosTurma);
                                for( $ct=0; $ct < $totalAlunos; $ct++)
                                {
                                    $numero = $ct + 1;
                                    echo '<tr>';
                                    echo "<td>{$numero}</td>";
                                    echo "<td>{$arrsqAlunoTurmaTransferido[$ct]['caeid']}</td>";
                                    echo "<td class=\"nomeAluno\">{$arrsqAlunoTurmaTransferido[$ct]['caenome']}</td>";
//ver($arrsqAlunoTurmaTransferido[$ct]['caeid']);
                                    $ctTrabalho     = 0;
                                   
                                    foreach( $arrTrabalhosTurma as $trabalhoTurma )
                                    {   
                                        $difId  = $trabalhoTurma['difid'];
                                        $caeid2  = $arrsqAlunoTurmaTransferido[$ct]['caeid'];
//                                       ver($cad_caeid);
//                                         if( $ctTrabalho == ($totalTrabalho-1) )
                                            echo '<td style="border-right: 1px solid black;">', geraCombo2( $cad_caeid, $difId, $dadosDiario['esdid'], $arrTrabalhoAlunoTurmatransferido[$caeid2][$difId] ), '</td>';
//                                         else
//                                             echo '<td>', geraCombo2( $cad_caeid, $difId, $dadosDiario['esdid'], $arrTrabalhoAlunoTurmatransferido[$caeid2][$difId] ), '</td>';
 
                                        $ctTrabalho++;

                                    }

                                    echo '</tr>';
                                }
                            }
                            ?>
                        </tbody>
                    </table>
                     <input type="hidden" name="acao" id="acao" value="" />
                     <input type="hidden" name="diaid" id="diaid" value="<?php echo $dadosDiario['diaid'];?>" />
                 </form>
             </td>
         </tr>
         <tr>
             <td colspan="5" style="padding-top: 15px;">
                 A fidedignidade das informações registradas neste Diário são de responsabilidade do(s) Educadores(as).
             </td>
         </tr>
    </table>
    <div id="btnImpressao" style="width: 100%; text-align: right;"><?ver(WF_ESTADO_DIARIO_ABERTO);?>
        <?php if( ($dadosDiario['esdid'] == WF_ESTADO_DIARIO_ABERTO && (!in_array( PFL_CONSULTA, $perfis )||$db->testa_superuser()))) { ?>
            <input type="button" name="btnSalvarTrabalho" id="btnSalvarTrabalho" value="Salvar" />
            <input type="button" name="btnFecharTrabalho" id="btnFecharTrabalho" value="Fechar Diários" />
        <?php } ?>
    </div>
</div>
