<?php
require_once APPRAIZ . 'www/projovemurbano/_funcoes.php';

$difId  = (int) $_REQUEST['difid'];

$sqlDadosDiario = "SELECT dif.difid, coc.cocnome, coc.cocqtdhoras, tur.turdesc
                        , unf.unfdesc, cic.cicdesc, nuc.nucid, ent.entnome
                        , TO_CHAR( per.perdtinicio, 'DD/MM/YYYY') as perdtinicio
                        , TO_CHAR( per.perdtfim, 'DD/MM/YYYY') as perdtfim                        
                        , ende.endlog, ende.endbai, ende.estuf, ende.endnum
                        , pmun.mundsc
                    FROM projovemurbano.diario dia
                    INNER JOIN projovemurbano.diariofrequencia dif
                        ON dia.diaid = dif.diaid
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
                    WHERE dif.difid = {$difId}
                    AND coc.cocdisciplina = 'D' ";

$sqAlunoTurma = "SELECT cae.caeid, cae.caenome
                FROM projovemurbano.diariofrequencia dif
                INNER JOIN projovemurbano.diario dia
                    ON dif.diaid = dia.diaid
                INNER JOIN projovemurbano.turma tur
                    ON dia.turid = tur.turid
                INNER JOIN projovemurbano.cadastroestudante cae
                    ON tur.turid = cae.turid
                WHERE dif.difid = {$difId}
                AND cae.caestatus = 'A' 
                ORDER BY cae.caenome";

$arrAlunosTurma = $db->carregar($sqAlunoTurma);
$dadosDiario     = $db->pegaLinha($sqlDadosDiario);

//municipial
if($_SESSION['projovemurbano']['muncod'])
{
    $sql = "SELECT '1' as codigo, mu.mundescricao, mu.estuf
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
            AND dif.difid       = {$difId}";
    $dado = $db->pegaLinha($sql);
    
}

//estadual
if($_SESSION['projovemurbano']['estuf'])
{
    $sql = "SELECT '2' as codigo, mu.estuf
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
            AND dif.difid = {$difId}";
            
    $dado               = $db->pegaLinha($sql);
    $pmupossuipolo = $db->pegaUm("SELECT pmupossuipolo FROM projovemurbano.polomunicipio pmu WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
    
    if( $pmupossuipolo == 't' )
        $dado['polnumero']  = $db->pegaUm("SELECT polid FROM projovemurbano.associamucipiopolo WHERE munid='".$dado['munid']."'") . ' - '. $dado['mundescricao'] .' - '. $dado['estuf'];	
}

$sqlDadosInstituicao = "SELECT DISTINCT ee.entnome, ee.entcodent
                        FROM projovemurbano.usuarioresponsabilidade ur
                        INNER JOIN  projovemurbano.nucleoescola ne ON ne.entid = ur.entid and ne.nuestatus='A'
                        INNER JOIN  entidade.entidade ee ON ee.entid = ur.entid
                        where ur.rpustatus='A'
                        AND ur.usucpf = '{$_SESSION['usucpf']}'";

$dadosInstituicao       = $db->pegaLinha($sqlDadosInstituicao);

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
                        <td rowspan="4" style="width: 150px;"> <img  src="../imagens/projovemurbano_cinza.jpg" alt="PROJOVEM URBANO" title="PROJOVEMURBANO" /> </td>
                        <td class="textoCentro"> Ministério da Educação </td>
                    </tr>
                    <tr>
                        <td class="textoCentro"> Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão </td>
                    </tr>
                    <tr>
                        <td class="textoCentro">DIÁRIO DE FREQUÊNCIA</td>
                    </tr>
                    <tr>
                        <td class="textoCentro" style="font-size: 20px;"><strong>Componente Curricular: <?php echo $dadosDiario['cocnome'] ?></strong></td>
                    </tr>
                </table>
            </td>
        </tr>
        
        
        <!-- Informacoes -->
        <tbody>
            <tr>
                <td class="colunaLabel">Coordenação Distrital/Estadual/Municipal:</td>
                <td style="width: 800px;" colspan="2"><?php echo montaTituloEstMun(); ?></td>
                <td class="colunaLabel">Aulas Previstas: <?php echo $dadosDiario['cocqtdhoras']; ?> horas </td>
                <td> </td>
            </tr>
            <tr>
                <td class="colunaLabel">Pólo:</td>
                <td colspan="2"><?php echo $dado['polnumero']; ?> </td>
                <td></td>
                <td></td>
            </tr>
            <tr>
                <td class="colunaLabel">Núcleo:</td>
                <td colspan="2"><?php echo 'Escola: ', $dadosInstituicao['entnome'], ' - Núcleo: ', $dadosDiario['nucid']; ?></td>
                <td class="colunaLabel">Aulas Dadas: ______</td>
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
                <td style="width: 200px;"><?php echo $dadosDiario['turdesc']; ?></td>
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
                 <table style="width: 100%; " id="presenca" cellspacing="0" cellpadding="0" border="0">
                     <thead>
                     <td class="textoCentro" style="width: 200px; " >Nº</td>
                        <td class="textoCentro" style="width: 200px;">Matrícula</td>
                        <td class="textoCentro nomeAluno" style="height: 100px;">Estudante</td>
                        <?php
                        foreach( range(1, 25) as $numero )
                        {
                            echo '<td class="colunaPresenca">&nbsp;</td>';
                        }
                        ?>
                        <td class="textoCentro colunaPresenca" style="border-right: 1px solid black;">Nº Presenças</td>
                     </thead>
                     <tbody>
                         <?php
                         if( $arrAlunosTurma == false ) {
                             echo '<td colspan="30" style="border-right: 1px solid black;">Não foram encontrados alunos na turma</td>';
                         }else {
                            $totalAlunos = count($arrAlunosTurma);
                            $numero = 0;
                            for( $ct=0; $ct < $totalAlunos; $ct++)
                            {
                                $numero = $ct + 1;
                                echo '<tr>';
                                echo "<td>{$numero}</td>";
                                echo "<td>{$arrAlunosTurma[$ct]['caeid']}</td>";
                                echo "<td class=\"nomeAluno\">{$arrAlunosTurma[$ct]['caenome']}</td>";

                                foreach( range(1, 25) as $numero )
                                {
                                    echo '<td>&nbsp;</td>';
                                }

                                echo '<td class="colunaPresenca" style="border-right: 1px solid black;">&nbsp;</td>';
                                echo '</tr>';
                            }
                         }
                         ?>
                     </tbody>
                 </table>
             </td>
         </tr>
         <tr>
             <td colspan="5" style="padding-top: 15px;">
                 A fidedignidade das informações registradas neste Diário são de responsabilidade do Educador(a):_______________________________________________________________________
             </td>
         </tr>
         <tr>
             <td colspan="5" style="padding-top: 15px;" >
                 Assinatura:_______________________________________________________________________
             </td>
         </tr>
    </table>
    <div id="btnImpressao" style="width: 100%; text-align: right;" >
        <input type="button" name="btnImprimirDiario" id="btnImprimirDiario" value="Imprimir" />
        <input type="button" name="btnVoltar" id="btnVoltar" value="Voltar" />
        <input type="hidden" name="diario_id" id="diario_id" value="<?php echo $difId; ?>" />
    </div>
</div>
<script type="text/javascript">
    
    if( typeof($) == 'function' ){
        $(document).ready(function(){

            $('#btnImprimirDiario').click(function(){
                var difId = $('#diario_id').val();
                window.open( 'projovemurbano.php?modulo=principal/diarioImpresso&acao=A&tipo=D&difid='+ difId, 'teste' );
            });
            
            $('#btnVoltar').click(function(){
                $('#btnVisualizarDiario').trigger('click');
            });
        });
    }else{
        window.print();
        window.close();
    }
    
</script>