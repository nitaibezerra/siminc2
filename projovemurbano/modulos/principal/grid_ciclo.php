<?php
$turId  = (int) $_REQUEST['turid'];
$cocId  = (int) $_REQUEST['cocid'];
$ppuId  = (int) $_REQUEST['ppuid'];
$perId  = (int) $_REQUEST['perid'];

if ( !empty ($perId) && $perId != 0) {
    
    $perId = "AND per.perid  = {$perId}";
}

else {
    
    $perId = '';
    
}

$sqlGridDiarioPeriodo = "SELECT  cic.cicid, cic.cicdesc, unf.unfid, unf.unfdesc
                                , per.perdesc, per.perid
                                , aluno_turma_periodo.qtd_aluno_turma
                                
                                , COALESCE(periodo_componente_frequencia.stddesc, 'Não Gerado' ) as stddesc_frequencia                              
                                , periodo_componente_frequencia.difid as difid_frequencia
                                
                                FROM projovemurbano.periodocurso per 
                                INNER JOIN projovemurbano.unidadeformativa unf 
                                    ON per.unfid = unf.unfid 
                                INNER JOIN projovemurbano.ciclocurso cic	 
                                    ON unf.cicid = cic.cicid 
                                LEFT JOIN 
                                    ( SELECT DISTINCT dia.turid,  COUNT ( cae.caeid ) as qtd_aluno_turma
                                             , per.perid
                                        FROM projovemurbano.periodocurso per 
                                        LEFT JOIN projovemurbano.diario dia
                                            ON per.perid = dia.perid
                                        LEFT JOIN projovemurbano.cadastroestudante cae 
                                            ON dia.turid = cae.turid                                        
                                        WHERE dia.turid 	= {$turId}                                          
                                        GROUP BY dia.turid, dia.diaid, per.perid
                                    ) as aluno_turma_periodo
                                    ON per.perid = aluno_turma_periodo.perid
                                LEFT JOIN 
                                    ( SELECT dif.difid, per.perid
                                                , dia.diaid
                                                , esd.esddsc as stddesc
                                        FROM projovemurbano.diario dia                                            
                                        LEFT JOIN workflow.documento doc
                                            ON dia.docid = doc.docid
                                        LEFT JOIN workflow.estadodocumento esd
                                            ON doc.esdid = esd.esdid
                                        LEFT JOIN projovemurbano.diariofrequencia dif 
                                            ON dia.diaid = dif.diaid
                                        LEFT JOIN  projovemurbano.periodocurso per 
                                            ON dia.perid = per.perid                                        
                                        LEFT JOIN  projovemurbano.gradecurricular grd
                                            ON dif.grdid = grd.grdid                                        
                                        WHERE dia.turid 	= {$turId}  
                                        AND grd.cocid 		= {$cocId} 
                                        AND doc.tpdid       = ". WORKFLOW_TIPODOCUMENTO_DIARIO ."
                                        GROUP BY dia.turid, dif.difid, per.perid
                                                 , dia.diaid, esd.esddsc
                                    ) as periodo_componente_frequencia
                                    ON per.perid = periodo_componente_frequencia.perid
                                WHERE per.perstatus 	= 'A' 
                                AND unf.unfstatus 	= 'A' 
                                AND cic.cidstatus 	= 'A' 
                                AND cic.ppuid 		= {$ppuId}  
                                 {$perId}
                              
                                ORDER BY cic.cicid, unf.unfid, per.perid";
                               
                                
//ver($sqlGridDiarioPeriodo);
                                
$arrDiarioPeriodo    = $db->carregar($sqlGridDiarioPeriodo);

?>

<style>
    .textoCentro{
        text-align: center;
    }
    
    #grid_ciclo tbody tr{
        background-color:  #F7F7F7;
    }
    
    #grid_ciclo tbody tr:hover{
        background-color:  #ffffcc;
    }
    
    .visualizarDiarioGrid{
        cursor: pointer;
    }
    
    .visualizarDiarioTrabalhoGrid{
        cursor: pointer;
    }
    
</style>
<?php
foreach( $arrDiarioPeriodo as $diarioPeriodo  )
            {
            if($diarioPeriodo['stddesc_frequencia'] == "Não Gerado"){
?>
            <p class="error">Ainda não existe diário gerado para esse componente curricular.</p>
<?php 
            }else{
?>
<table border="1" cellspacing="0" cellpading="2" style="width: 100%; color: 333333;" class="listagem" id="grid_ciclo">
    <thead>
        <tr>
            <th class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">CICLO</th>
            <th class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">UNIDADE FORMATIVA</th>
            <th class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Mês</th>
            <th class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Qtd. Alunos</th>
            <th class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Situação Diário de Frequência</th>
            <th class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ação</th>
        </tr>
    </thead>
    <tbody>
        
        <?php
        $cicloId    = '';
        $unidadeId  = '';
        
            
            foreach( $arrDiarioPeriodo as $diarioPeriodo  )
            {
                echo '<tr>';

                if( $cicloId != $diarioPeriodo['cicid'] )
                {
                    echo '<td rowspan="6" class="textoCentro">'. $diarioPeriodo['cicdesc'] .'</td>';
                    $cicloId = $diarioPeriodo['cicid'];
                }

                if( $unidadeId != $diarioPeriodo['unfid'] )
                {
                    echo '<td rowspan="3" class="textoCentro">'. $diarioPeriodo['unfdesc'] .'</td>';
                    $unidadeId = $diarioPeriodo['unfid'];
                }

                if( empty($diarioPeriodo['perdesc']) )
                {
                    $diarioPeriodo['perdesc'] = '&nbsp';
                }

                if( empty($diarioPeriodo['qtd_aluno_turma']) )
                {
                    $diarioPeriodo['qtd_aluno_turma'] = '&nbsp';
                }
                if( empty($diarioPeriodo['stddesc']) )
                {
                    $diarioPeriodo['stddesc'] = '&nbsp';
                }

                $colunas =  '<td class="textoCentro">'. $diarioPeriodo['perdesc']  .'</td>'
                    .'<td class="textoCentro">'. $diarioPeriodo['qtd_aluno_turma'] .'</td>'
                    .'<td class="textoCentro">'. $diarioPeriodo['stddesc_frequencia'] .'</td>'                  
                    .'<td class="textoCentro">';

                if( !empty( $diarioPeriodo['difid_frequencia'] ) )
                {
                    $colunas .= '<img src="../imagens/folder_user.png" alt="Diário de Frequência" title="Diário de Frequência" class="visualizarDiarioGrid" id="'. $diarioPeriodo['difid_frequencia'] .'" >&nbsp;';
                    $colunas .= '&nbsp;<img src="../imagens/page_attach.png" alt="Diário de Trabalhos" title="Diário de Trabalhos" class="visualizarDiarioTrabalhoGrid" id="'. $diarioPeriodo['difid_frequencia'] .'" >';
                }


                $colunas .= '&nbsp</td>';

                echo $colunas;

                echo '</tr>';
            }
        
        ?>
    </tbody>
</table>
<?php 
        }
    }
?>