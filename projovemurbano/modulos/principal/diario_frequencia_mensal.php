<?php  
require_once APPRAIZ . 'www/projovemurbano/_funcoes.php';

$perfis = pegaPerfilGeral();

$perid = (int) $_REQUEST['perid'];
$turid = (int) $_REQUEST['turid'];
$nucid = (int) $_REQUEST['nucid'];

$parametros = array( 'perid' => $perid
    				, 'turid' => $turid
    				, 'nucid' => $nucid );

$infoDiario = montaCabecalhoDoDiarioFrequenciaMensal( $parametros );
//ver($turid,d);
$sqlDadosDiario = "SELECT dia.diaid, doc.esdid, SUM ( coc.cocqtdhoras ) as total_horas
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
                    AND coc.cocdisciplina   = 'D'
                    AND dia.turid           = {$turId}
                    GROUP BY dia.diaid, doc.esdid";   
                       
$dadosDiario = $db->pegaLinha($sqlDadosDiario);

$habilitado = 'S';

$sqlDiarioComponenteGerado = "SELECT dia.diaid, COUNT( grd.grdid ) as total_diario_componente
                                FROM projovemurbano.diario dia
                                LEFT JOIN projovemurbano.diariofrequencia dif
                                    ON dia.diaid = dif.diaid
                                LEFT JOIN projovemurbano.gradecurricular grd
                                    ON dif.grdid = grd.grdid
                                LEFT JOIN projovemurbano.componentecurricular coc
                                    ON grd.cocid = coc.cocid                                    
                                WHERE dia.turid = {$turId}
                                AND dia.perid = {$perId}
                                AND coc.cocdisciplina = 'D'
                                GROUP BY dia.diaid";
                              
$sqlTotalComponente = "SELECT COUNT( coc.cocid ) as total_componente
                        FROM projovemurbano.componentecurricular coc
                        WHERE cocstatus = 'A'
                        AND coc.cocdisciplina = 'D'";
$totalComponente    = $db->pegaUm($sqlTotalComponente);                           
$diarioComponente   = $db->pegaLinha($sqlDiarioComponenteGerado);

if( $diarioComponente == false || $diarioComponente['total_diario_componente'] < $totalComponente )
{
    echo '<p>Para visualização do diário é necessário gerar o diário de todos os componentes curriculares</p>';
    exit;
}

?>
<style>
    .textoCentro{
        text-align: center;
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
    .fundoCinza{
        background-color: #cccccc;
    }
</style>

<div>
    <table id="tableFrequenciaMensal" style="width: 100%;" border="0" align="center">
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
                        <td class="textoCentro" style="font-size: 20px;"><strong>DIÁRIO DE FREQUÊNCIA</strong></td>
                    </tr>
                    <tr>
                        <td class="textoCentro"></td>
                    </tr>
                </table>
            </td>
        </tr>

        <!-- Informações -->
        <tbody>
            <tr>
                <td class="colunaLabel"><b>Coordenação Municipal:</b></td>
                <td colspan="2"><?php echo montaTituloEstMun(); ?></td>
                <td class="colunaLabel"><b>Aulas Previstas:</b> <?php echo( $dadosDiario['total_horas'] ); ?> Horas</td>
                <td> </td>
            </tr>
            <tr>
                <td class="colunaLabel"><b>Pólo:</b></td>
                <td colspan="2"><?php echo( $infoDiario[0]['polo'] ); ?></td>
                <td></td>
                <td></td>
            </tr>
            <tr>
                <td class="colunaLabel"><b>Núcleo:</b></td>
                <td colspan="2"><?php echo 'Escola: ', $infoDiario[0]['entidade'], ' - Núcleo: ', $infoDiario[0]['nucleo']; ?></td>
                <td class="colunaLabel"><b>Aulas Dadas:</b> <?php echo( $infoDiario[0]['soma_difqtdauladada'] ); ?> Horas</td>
                <td>&nbsp;</td>
            </tr>
            <tr>
                <td class="colunaLabel"><b>Endereço:</b></td>
                <td colspan="2"><?php echo( $infoDiario[0]['endereco_completo'] ); ?></td>
                <td></td>
                <td></td>
            </tr>
            <tr>
                <td class="colunaLabel"><b>Turma:</b></td>
                <td><?php echo( $infoDiario[0]['turma'] ); ?></td>
                <td class="mes" colspan="3"><b>Mês: <?php echo formata_data( $infoDiario[0]['dt_inicio'] ), ' a ', formata_data( $infoDiario[0]['dt_fim'] ); ?></b></td>
            </tr>
            <tr>
                <td class="colunaLabel"><b>Ciclo:</b></td>
                <td colspan="2"><?php echo $infoDiario[0]['ciclo']; ?></td>
                <td></td>
                <td></td>
            </tr>
            <tr>
                <td class="colunaLabel"><b>Unidade Formativa:</b></td>
                <td colspan="2"><?php echo $infoDiario[0]['unidade']; ?></td>
                <td></td>
                <td></td>
            </tr>
        </tbody>
        <tr>
        </tr>
        <!-- Componente Curricular -->
        <tr>
            <td colspan="5">
                <table style="width: 100%; " id="presenca" cellspacing="0" cellpadding="0" border="1">
                    <?php
                    $componentesCurriculares        = listarComponenteCurricular( $infoDiario[0]['diaid'] );
                    $qtdColunas                     = count( $componentesCurriculares );
                    $componenteCurricularCabecalho  = '<td colspan="3">&nbsp;</td>';
                    $componenteCurricularFormulario = '<td colspan="3">Aulas Dadas</td>';
                    $somaAulaDada                   = 0;
                    foreach ( $componentesCurriculares as $componenteCurricular ) {
                        
                        $componenteCurricularCabecalho.= "<td>";
                        $componenteCurricularCabecalho.= $componenteCurricular['cocnome'];
                        $componenteCurricularCabecalho.="</td>";
                        $qtdAulaDada = '0';

                        for ( $i = 0; $i < count( $infoDiario ); $i++ ) {
                            if ( $componenteCurricular['difid'] == $infoDiario[$i]['difid'] ) {
                                $qtdAulaDada = $infoDiario[$i]['difqtdauladada'];
                            }
                        }

                        // Desabilita Campo Texto
                        $habilitaCampoDiario = (!empty( $componenteCurricular['difid'] ) ? 'S' : 'N');
                        
                        if( $dadosDiario['esdid'] == WF_ESTADO_DIARIO_ABERTO )
                        {
                            $habilitaCampoDiario = 'S';
                        }else{
                            $habilitaCampoDiario = 'N';
                        }
                        
                        $somaAulaDada += $qtdAulaDada;
                        $componenteCurricularFormulario.= "<td>";
                        $idCampoAulasDatas   = "qtdaulasdadas[{$componenteCurricular['difid']}]";
                        $componenteCurricularFormulario.= campo_texto( "qtdaulasdadas[" . $componenteCurricular['difid'] . "]", 'N', $habilitaCampoDiario, 'Quantidade de aulas dadas', "2", "2", "[#]", "", "", "", "", 'id="' . $idCampoAulasDatas . '"', "", $qtdAulaDada );
                        $componenteCurricularFormulario.= "</td>";
                    }
//                     ver($componenteCurricularFormulario,d);
                    ?>
                    <tr>
                        <?php echo $componenteCurricularCabecalho; ?>
                        <td>Carga Horária Cumprida</td>
                    </tr>
                    <tr>
                        <?php echo $componenteCurricularFormulario; ?>
                        <td><?php echo $somaAulaDada ?></td>
                    </tr>
                    <tr>
                        <td class="fundoCinza">Nº</td>
                        <td class="fundoCinza">Matrícula</td>
                        <td class="fundoCinza">Aluno</td>
                        <?php
                        for ( $i                   = 0; $i < $qtdColunas; $i++ ) {
                            echo("<td class=\"fundoCinza\">&nbsp;</td>");
                        }
                        ?>
                        <td class="fundoCinza"></td>
                    </tr>
                    <?php
                    $listaDeEstudantes = listaEstudantesPorTurma( $infoDiario[0]['turma_id'] );
                    $numeroLista       = 1;
                    if ( $listaDeEstudantes == false ) {
                        $colspan = $qtdColunas + 4;
                        echo '<tr><td colspan="' . $colspan . '">Nenhum aluno ativo encontrado para a turma selecionada.</td><tr>';
                    } else {
                        // Lista os estudantes
                        foreach ( $listaDeEstudantes as $estudantes ) {
								$sql = "SELECT distinct
                    						true
										FROM projovemurbano.frequenciaestudante frq
										INNER JOIN projovemurbano.diariofrequencia dif ON frq.difid = dif.difid
										WHERE 
											frq.caeid = {$estudantes['caeid']}
										AND 	dif.diaid =  {$infoDiario[0]['diaid']}";
								$teste = $db->pegaUm ( $sql );
// 							if($estudantes['caenome'] == 'EDIVAN CARVALHO DA SILVA'){
// 								ver($teste,$estudantes['caestatus'],$infoDiario[0]['diaid']);
// 							}
							if($estudantes['caestatus']== 'A'||$teste){
	                            echo "<tr>";
	                            echo "<td>" . $numeroLista . "</td>";
	                            echo "<td>" . $estudantes['caeid'] . "</td>";
	                            echo "<td>" . $estudantes['caenome'] . "</td>";
	                            $somaQuantidadePresenca = 0;
	
	                            // Cria as celulas de acordo com a quantidade de componentes curriculares
	                            for ( $i = 0; $i < $qtdColunas; $i++ ) {
	
	                                $parametrosPresenca = array( 'difid' => $componentesCurriculares[$i]['difid']
	                                    , 'caeid' => $estudantes['caeid'] );
	//                                    ver($parametrosPresenca);
	                                // Recupera a presenca por diário de frequencia 
	                                $dadosPresenca = listaPresencaPorAluno( $parametrosPresenca );
	
	                                // Habilita/Desabilita campo de edição de presença
	//                                 $habilitaPresenca = (!empty( $componentesCurriculares[$i]['difid'] ) ? 'S' : 'N');
	                                
	                                if( ($dadosDiario['esdid'] == WF_ESTADO_DIARIO_ABERTO) && ($estudantes['caestatus']== 'A'))
	                                {
	                                    $habilitaPresenca = 'S';
	                                    $disable = '';
	                                }else{
	                                    $habilitaPresenca = 'N';
	                                    $disable = 'disabled="disabled"';
	                                }
	
	                                // Criação de parametros para o Array HTML
	                                $frqIdDoEstudante = (!empty( $dadosPresenca['frqid'] ) ? $dadosPresenca['frqid'] : '0' );
	                                $caeIdDoEstudante = $estudantes['caeid'];
	                                $difIdDoEstudante = (!empty( $componentesCurriculares[$i]['difid'] ) ? $componentesCurriculares[$i]['difid'] : '0' );
	                                $qtdPresenca      = (!empty( $dadosPresenca['frqqtdpresenca'] ) ? $dadosPresenca['frqqtdpresenca'] : '0' );
	                                $somaQuantidadePresenca += $qtdPresenca;
	
	                                echo "<td>";
	                                //var_dump( $dadosPresenca );
	                                $classComCur = "class='qtdaulas_{$difIdDoEstudante}'";
	                                $idCampo     = "qtdaulas_{$difIdDoEstudante}_{$caeIdDoEstudante}_{$frqIdDoEstudante}";
	                                echo campo_texto( "qtdaulas[" . $difIdDoEstudante . "][" . $caeIdDoEstudante . "][" . $frqIdDoEstudante . "]", 'N', $habilitaPresenca, 'Quantidade de aulas dadas', "2", '2', '[#]', '', "", "", "", $disable.'id="' . $idCampo . '" ' . $classComCur, "", $qtdPresenca );
	                                echo "</td>";
	
	                                //$idFrequenciaEstudante = '';
	                            }
	                            echo "<td>{$somaQuantidadePresenca}</td>";
	                            echo "</tr>";
	                            $numeroLista++;
	                        }
                        }
                    }
                    ?>
                     <?php
                    $componentesCurricularesTrans        = listarComponenteCurricularTrans( $infoDiario[0]['diaid']  );
                    
                    $qtdColunas                     = count( $componentesCurricularesTrans );
                    $componenteCurricularCabecalho  = '<td colspan="3">&nbsp;</td>';
                    $componenteCurricularFormulario = '<td colspan="3">Aulas Dadas</td>';
                    $somaAulaDada                   = 0;
                    foreach ( $componentesCurricularesTrans as $componenteCurricularTrans ) {
                        
                        $componenteCurricularCabecalho.= "<td>";
                        $componenteCurricularCabecalho.= $componenteCurricularTrans['cocnome'];
                        $componenteCurricularCabecalho.="</td>";
                        $qtdAulaDada = '0';

                        for ( $i = 0; $i < count( $infoDiario ); $i++ ) {
                            if ( $componenteCurricularTrans['difid'] == $infoDiario[$i]['difid'] ) {
                                $qtdAulaDada = $infoDiario[$i]['difqtdauladada'];
                            }
                        }

                        // Desabilita Campo Texto
                        $habilitaCampoDiario = (!empty( $componenteCurricularTrans['difid'] ) ? 'N' : 'N');
                        
                        /*if( $dadosDiario['esdid'] == WF_ESTADO_DIARIO_ABERTO )
                        {
                            $habilitaCampoDiario = 'S';
                        }else{*/
                            $habilitaCampoDiario = 'N';
                       /* }*/
                        
                        $somaAulaDada += $qtdAulaDada;
                        $componenteCurricularFormulario.= "<td>";
                        $idCampoAulasDatas   = "qtdaulasdadas[{$componenteCurricularTrans['difid']}]";
//                         $aulasdadas = "qtdaulasdadas[" . $componenteCurricular['difid'] . "]";
                        $componenteCurricularFormulario.= campo_texto( "$aulasdadas", 'N', $habilitaCampoDiario, 'Quantidade de aulas dadas', "2", "2", "[#]", "", "", "", "", 'id="' . $idCampoAulasDatas . '"', "",$qtdAulaDada );
                        $componenteCurricularFormulario.= "</td>";
                    }
                    ?>
                    
                <tr>
                    <td colspan="5" style="padding-top: 15px;">
                        <b>Alunos que foram transferidos.</b>
                    </td>
                </tr>
                <tr>
                    <?php echo $componenteCurricularCabecalho; ?>
                    <td>Carga Horária Cumprida</td>
                </tr>
                <tr>
                    <?php echo $componenteCurricularFormulario; ?>
                    <td><?php echo $somaAulaDada ?></td>
                </tr>
                <tr>
                    <td class="fundoCinza">Nº</td>
                    <td class="fundoCinza">Matrícula</td>
                    <td class="fundoCinza">Aluno</td>
                    <?php
                    for ( $i                   = 0; $i < $qtdColunas; $i++ ) {
                        echo("<td class=\"fundoCinza\">&nbsp;</td>");
                    }
                    ?>
                    <td class="fundoCinza"></td>
                </tr>
                 <?php
                 
                 $listaDeEstudantesTransferidos = listaEstudantesTransferidosPorTurma( $infoDiario[0]['turma_id'] );
                 $numeroLista       = 1;
                 
                    if ( $listaDeEstudantesTransferidos == false ) {
                        $colspan = $qtdColunas + 4;
                        echo '<tr><td colspan="' . $colspan . '"></td><tr>';
                    } else {
                        // Lista os estudantes que foram transferidos
                       
                        foreach ( $listaDeEstudantesTransferidos as $estudantestransferidos ) {
                            
                            echo "<tr>";
                            echo "<td>" . $numeroLista . "</td>";
                            echo "<td>" . $estudantestransferidos['caeid'] . "</td>";
                            echo "<td>" . $estudantestransferidos['caenome'] . "</td>";
                            $somaQuantidadePresenca = 0;

                            // Cria as celulas de acordo com a quantidade de componentes curriculares
                            for ( $i = 0; $i < $qtdColunas; $i++ ) {

                                $parametrosPresencaTrans = array( 'difid' => $componentesCurricularesTrans[$i]['difid']
                                    , 'caeid' => $estudantestransferidos['caeid'] );
                                    
                                // Recupera a presenca por diário de frequencia 
                                $dadosPresencaTrans = listaPresencaPorAlunoTransferido( $parametrosPresencaTrans );
//                     ver($dadosPresencaTrans,d);           
                                // Habilita/Desabilita campo de edição de presença
                                $habilitaPresenca = (!empty( $componentesCurricularesTrans[$i]['difid'] ) ? 'N' : 'N');
                                
                                    $habilitaPresenca = 'N';

                                // Criação de parametros para o Array HTML
                                $frqIdDoEstudanteTrans = (!empty( $dadosPresencaTrans['frqid'] ) ? $dadosPresencaTrans['frqid'] : '0' );
                                $caeIdDoEstudanteTrans = $estudantestransferidos['caeid'];
                                $difIdDoEstudanteTrans = (!empty( $componentesCurricularesTrans[$i]['difid'] ) ? $componentesCurricularesTrans[$i]['difid'] : '0' );
                                $qtdPresenca      = (!empty( $dadosPresencaTrans['frqqtdpresenca'] ) ? $dadosPresencaTrans['frqqtdpresenca'] : '0' );
                                $somaQuantidadePresenca += $qtdPresenca;

                                echo "<td>";
                                //var_dump( $dadosPresenca );
                                $classComCur = "class='qtdaulas_{$difIdDoEstudanteTrans}'";
                                $idCampo     = "qtdaulas_{$difIdDoEstudanteTrans}_{$caeIdDoEstudanteTrans}_{$frqIdDoEstudanteTrans}";
//                                 $vlr = "qtdaulas[" . $difIdDoEstudanteTrans . "][" . $caeIdDoEstudanteTrans . "][" . $frqIdDoEstudanteTrans . "]";
                                echo campo_texto( "$vlr", 'N', $habilitaPresenca, 'Quantidade de aulas dadas', "2", '2', '[#]', '', "", "", "", "", "", $qtdPresenca );
                                echo "</td>";

                                //$idFrequenciaEstudante = '';
                            }
                            echo "<td>{$somaQuantidadePresenca}</td>";
                            echo "</tr>";
                            $numeroLista++;
                             // Lista os estudantes que foram transferidos
                        
                        }
                    }
                    ?>
                </table>
                <input type="hidden" name="qtd_aula_prevista"  id="qtd_aula_prevista" value="<?php echo $infoDiario[0]['soma_difqtdaulaprevista']; ?>" />
            </td>
        </tr>
        <tr>
            <td colspan="5" style="padding-top: 15px;">
                <b>A fidedignidade das informações registradas neste Sistema são de responsabilidade do Diretor(a) da unidade escolar.</b>
            </td>
        </tr>
    </table>
    <div id="btnImpressao" style="width: 95%; text-align: right;" >
        <?php 
        if( $dadosDiario['esdid'] == WF_ESTADO_DIARIO_ABERTO && !in_array( PFL_CONSULTA, $perfis ) ){ ?>
                <input type="button" name="salvarDiarioFrequenciaMensal" id="salvarDiarioFrequenciaMensal" value="Salvar" />
        <?php } ?>
    </div>
</div>