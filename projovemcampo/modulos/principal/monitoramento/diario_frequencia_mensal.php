<?php  
require_once APPRAIZ . 'www/projovemcampo/_funcoes.php';

$perfis = pegaPerfilGeral();
$arrPerRap = spliti('-',$_REQUEST['perid'],2);
$perid = (int) $arrPerRap[0];
$turid = (int) $_REQUEST['turid'];
$entid = (int) $_REQUEST['entid'];

$parametros = array( 'perid' => $perid
    				, 'turid' => $turid
    				, 'entid' => $entid );

$infoDiario = montaCabecalhoDoDiarioFrequenciaMensal( $parametros );
$sqlDadosDiario = "SELECT 
						dia.diaid, 
						diatempoescola as tempoescola,
						diatempocomunidade as tempocomunidade,
						stdid as status
					FROM  
						projovemcampo.diario dia
					INNER JOIN projovemcampo.historico_diario hid ON hid.hidid = dia.hidid
					WHERE
							turid = {$turid}
					AND 	perid = {$perid}";   
                       
$dadosDiario = $db->pegaLinha($sqlDadosDiario);

$sqlcoordturma="SELECT
					true
				FROM
					projovemcampo.planoprofissional ppr
				INNER JOIN projovemcampo.planodeimplementacao pim ON pim.pimid = ppr.pimid
				WHERE
					apcid ={$_SESSION['projovemcampo']['apcid']}";
$temcoordturma = $db->pegaUm ( $sqlcoordturma );

if($dadosDiario['status'] == 1 ||$dadosDiario['status'] == 5){
	$habilitado = 'S';
}else{
	$habilitado = 'N';
}

if(!$db->testa_superuser()){
	$habilitado = 'N';
	if($temcoordturma==true){
		if((in_array(PFL_DIRETOR_ESCOLA, $perfis)||in_array( PFL_ADMINISTRADOR, $perfis ))&&$dadosDiario['status']==1){
			$habilitado = 'S';
		}elseif((in_array(PFL_DIRETOR_ESCOLA, $perfis)||in_array( PFL_ADMINISTRADOR, $perfis ))&&$dadosDiario['status']==5){
			$habilitado = 'S';
		}
	}else{
		if((in_array(PFL_DIRETOR_ESCOLA, $perfis)||in_array( PFL_ADMINISTRADOR, $perfis ))&&$dadosDiario['status']==1){
			$habilitado = 'S';
		}elseif((in_array(PFL_DIRETOR_ESCOLA, $perfis)||in_array( PFL_ADMINISTRADOR, $perfis ))&&$dadosDiario['status']==5){
			$habilitado = 'S';
		}
	}
}

$diarioComponente = array(
							        0 => array("codigo" => "E", "descricao" => "Tempo Escola"),
							        1 => array("codigo" => "C", "descricao" => "Tempo Comunidade"));
//src="../imagens/projovemurbano_cinza.jpg"
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
                        <td rowspan="4" style="width: 150px;"> <img  alt="PROJOVEM CAMPO" title="projovemcampo" style="font-size: 15px;" /> </td>
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
                <td class="colunaLabel"><b><?if($_SESSION['projovemcampo']['estuf']){?>Coordenação Estadual:<? }else{?>Coordenação Municipal:<?}?></b></td>
                <td colspan="2"><?php echo montaTituloEstMun(); ?></td>
                <td class="colunaLabel"></td>
                <td> </td>
            </tr>
            <tr>
                <td class="colunaLabel"><b>Escola:</b></td>
                <td colspan="2"><?php echo( $infoDiario['entidade'] ); ?></td>
                <td class="colunaLabel"></td>
                <td>&nbsp;</td>
            </tr>
            <tr>
                <td class="colunaLabel"><b>Endereço:</b></td>
                <td colspan="2"><?php echo( $infoDiario['endereco_completo'] ); ?></td>
                <td></td>
                <td></td>
            </tr>
            <tr>
                <td class="colunaLabel"><b>Turma:</b></td>
                <td><?php echo( $infoDiario['turma'] ); ?></td>
                <td class="mes" colspan="3"><b>Mês: <?php echo formata_data( $infoDiario['dt_inicio'] ), ' a ', formata_data( $infoDiario['dt_fim'] ); ?></b></td>
            </tr>
        </tbody>
        <!-- Componente Curricular -->
        <tr>
            <td colspan="5">
                <table style="width: 100%; " id="presenca" cellspacing="0" cellpadding="0" border="1">
                    <?php
                    
                    $componentesCurriculares        = $diarioComponente;
                    $qtdColunas                     = count( $componentesCurriculares );
                    $componenteCurricularCabecalho  = '<td colspan="3">&nbsp;</td>';
                    $componenteCurricularFormulario = '<td colspan="3">Aulas Dadas</td>';
                    $somaAulaDada                   = 0;
                    foreach ( $componentesCurriculares as $componenteCurricular ) {
                        
                        $componenteCurricularCabecalho.= "<td>";
                        $componenteCurricularCabecalho.= $componenteCurricular['descricao'];
                        $componenteCurricularCabecalho.="</td>";

						if($componenteCurricular['codigo'] == 'E'){
                        	$qtdAulaDada = $infoDiario['tempoescola']?$infoDiario['tempoescola']:0;
						}else{
							$qtdAulaDada = $infoDiario['tempocomunidade']?$infoDiario['tempocomunidade']:0;
						}
                        // Desabilita Campo Texto
//                         $habilitaCampoDiario = (!empty( $dadosDiario['diaid'] ) ? 'S' : 'N');
                        
//                         if( $dadosDiario['status'] == 1 )
//                         {
//                             $habilitaCampoDiario = 'S';
//                         }else{
//                             $habilitaCampoDiario = 'N';
//                         }
                        
                        $somaAulaDada += $qtdAulaDada;
                        $componenteCurricularFormulario.= "<td>";
                        $idCampoAulasDatas   = "qtdaulasdadas_{$componenteCurricular['codigo']}";
                        $componenteCurricularFormulario.= campo_texto( "qtdaulasdadas[".$componenteCurricular['codigo']."]", 'N', $habilitado, 'Quantidade de aulas dadas', "3", "3", "[#]", "", "", "", 
																		"", ' id="' . $idCampoAulasDatas . '"  codigoensino="'.$componenteCurricular['codigo'].'"   class="somaaulasdadas" ', "", $qtdAulaDada );
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
                        <td><span id ='totalaulasdadas'><?php echo $somaAulaDada ?></td>
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
                    $listaDeEstudantes = listaEstudantesPorTurma( $infoDiario['turma_id'] );
                    $numeroLista       = 1;
                    if ( $listaDeEstudantes == false ) {
                        $colspan = $qtdColunas + 4;
                        echo '<tr><td colspan="' . $colspan . '">Nenhum aluno ativo encontrado para a turma selecionada.</td><tr>';
                    } else {
                        // Lista os estudantes
                        foreach ( $listaDeEstudantes as $estudantes ) {
// 								$sql = "SELECT distinct
// 											true
// 										FROM 
// 											projovemcampo.diario dia
// 										LEFT JOIN projovemcampo.lancamentodiario lnd ON lnd.diaid = dia.diaid
// 										WHERE 
// 											lnd.estid = {$estudantes['estid']}
// 										AND 	lnd.diaid =  {$infoDiario['diaid']}";
// 								$teste = $db->pegaUm ( $sql );
// 							ver();
							if($estudantes['eststatus']== 'A'/*||$teste*/){
	                            echo "<tr>";
	                            echo "<td>" . $numeroLista . "</td>";
	                            echo "<td>" . $estudantes['estid'] . "</td>";
	                            echo "<td>" . $estudantes['estnome'] . "</td>";
	                            $somaQuantidadePresenca = 0;
	                           
	                            // Cria as celulas de acordo com a quantidade de componentes curriculares
	                            
	                            for ( $i = 0; $i < $qtdColunas; $i++ ) {
	                                $parametrosPresenca = array( 'diaid' => $dadosDiario['diaid']
	                                    						, 'estid' => $estudantes['estid'] );

	                                // Recupera a presenca por diário de frequencia 
	                                
	                                $dadosPresenca = listaPresencaPorAluno( $parametrosPresenca );
	                                
	                                // Habilita/Desabilita campo de edição de presença
	                                
	                                if( ($dadosDiario['status'] == 1||$dadosDiario['status'] == 5) && ($estudantes['eststatus']== 'A'))
	                                {
// 	                                    $habilitaPresenca = 'S';
	                                    $disable = '';
	                                }else{
// 	                                    $habilitaPresenca = 'N';
	                                    $disable = 'disabled="disabled"';
	                                }
	                                if($i == null){
	                                	$qtdPresenca      = (!empty( $dadosPresenca['lndhorasescola'] ) ? $dadosPresenca['lndhorasescola'] : '0' );
	                                	$comp = 'E';
	                                }else{
										$qtdPresenca      = (!empty( $dadosPresenca['lndhorascomunidade'] ) ? $dadosPresenca['lndhorascomunidade'] : '0' );
	                                	$comp = 'C';
	                                }
	                                // Criação de parametros para o Array HTML
// 	                                $lndIdDoEstudante = (!empty( $dadosPresenca['lndid'] ) ? $dadosPresenca['lndid'] : '0' );
	                                $estidDoEstudante = $estudantes['estid'];
	                                $diaidDoEstudante = (!empty( $dadosDiario['diaid'] ) ? $dadosDiario['diaid'] : '0' );
	                                $somaQuantidadePresenca += $qtdPresenca;
									
	                                echo "<td>";
	                                $classComCur = "class='qtdaulas_{$comp}'";
	                                $idCampo     = "{$estidDoEstudante}_{$comp}";
	                                echo campo_texto( "qtdaulas[" . $estidDoEstudante . "][" . $comp . "]", 'N', $habilitado, 'Quantidade de aulas dadas', "3", '3', '[#]', '', "", "", "", $disable.'  id="'.$idCampo.'"  idestudante="'.$estidDoEstudante.'"   '.$classComCur.'  class_2="somaaulasfrequentadas"   ', "", $qtdPresenca );
	                                echo "</td>";
	
	                                //$idFrequenciaEstudante = '';
	                            }
	                            echo "<td><span id ='somaQuantidadePresenca_".$estidDoEstudante."'>{$somaQuantidadePresenca}</td>";
	                            echo "</tr>";
	                            $numeroLista++;
	                        }
                        }
                    }
                    ?>
                     <?php
//                     $componentesCurricularesTrans        = listarComponenteCurricularTrans( $infoDiario[0]['diaid']  );
                    
//                     $qtdColunas                     = count( $componentesCurricularesTrans );
//                     $componenteCurricularCabecalho  = '<td colspan="3">&nbsp;</td>';
//                     $componenteCurricularFormulario = '<td colspan="3">Aulas Dadas</td>';
//                     $somaAulaDada                   = 0;
//                     foreach ( $componentesCurricularesTrans as $componenteCurricularTrans ) {
                        
//                         $componenteCurricularCabecalho.= "<td>";
//                         $componenteCurricularCabecalho.= $componenteCurricularTrans['descricao'];
//                         $componenteCurricularCabecalho.="</td>";
//                         $qtdAulaDada = '0';

//                         for ( $i = 0; $i < count( $infoDiario ); $i++ ) {
//                             if ( $componenteCurricularTrans['diaid'] == $infoDiario[$i]['diaid'] ) {
//                                 $qtdAulaDada = $infoDiario[$i]['difqtdauladada'];
//                             }
//                         }

//                         // Desabilita Campo Texto
//                         $habilitaCampoDiario = (!empty( $componenteCurricularTrans['diaid'] ) ? 'N' : 'N');
                        
//                         /*if( $dadosDiario['esdid'] == WF_ESTADO_DIARIO_ABERTO )
//                         {
//                             $habilitaCampoDiario = 'S';
//                         }else{*/
//                             $habilitaCampoDiario = 'N';
//                        /* }*/
                        
//                         $somaAulaDada += $qtdAulaDada;
//                         $componenteCurricularFormulario.= "<td>";
//                         $idCampoAulasDatas   = "qtdaulasdadas[{$componenteCurricularTrans['diaid']}]";
// //                         $aulasdadas = "qtdaulasdadas[" . $componenteCurricular['diaid'] . "]";
//                         $componenteCurricularFormulario.= campo_texto( "$aulasdadas", 'N', $habilitaCampoDiario, 'Quantidade de aulas dadas', "2", "2", "[#]", "", "", "", "", 'id="' . $idCampoAulasDatas . '"', "",$qtdAulaDada );
//                         $componenteCurricularFormulario.= "</td>";
//                     }
                    ?>
                    
<!--                 <tr> -->
<!--                      <td colspan="5" style="padding-top: 15px;">-->
<!--                         <b>Alunos que foram transferidos.</b> -->
<!--                     </td> -->
<!--                 </tr> -->
<!--                 <tr> -->
                    <?php //echo $componenteCurricularCabecalho; ?>
<!--                     <td>Carga Horária Cumprida</td> -->
<!--                 </tr> -->
<!--                 <tr> -->
                    <?php //echo $componenteCurricularFormulario; ?>
                    <td><?php //echo $somaAulaDada ?></td>
<!--                 </tr> -->
<!--                 <tr> -->
<!--                     <td class="fundoCinza">Nº</td> -->
<!--                     <td class="fundoCinza">Matrícula</td> -->
<!--                     <td class="fundoCinza">Aluno</td> -->
                    <?php
//                     for ( $i                   = 0; $i < $qtdColunas; $i++ ) {
//                         echo("<td class=\"fundoCinza\">&nbsp;</td>");
//                     }
//                     ?>
<!--                     <td class="fundoCinza"></td> -->
<!--                 </tr> -->
                 <?php
                 
//                  $listaDeEstudantesTransferidos = listaEstudantesTransferidosPorTurma( $infoDiario[0]['turma_id'] );
//                  $numeroLista       = 1;
                 
//                     if ( $listaDeEstudantesTransferidos == false ) {
//                         $colspan = $qtdColunas + 4;
//                         echo '<tr><td colspan="' . $colspan . '"></td><tr>';
//                     } else {
//                         // Lista os estudantes que foram transferidos
                       
//                         foreach ( $listaDeEstudantesTransferidos as $estudantestransferidos ) {
                            
//                             echo "<tr>";
//                             echo "<td>" . $numeroLista . "</td>";
//                             echo "<td>" . $estudantestransferidos['estid'] . "</td>";
//                             echo "<td>" . $estudantestransferidos['caenome'] . "</td>";
//                             $somaQuantidadePresenca = 0;

//                             // Cria as celulas de acordo com a quantidade de componentes curriculares
//                             for ( $i = 0; $i < $qtdColunas; $i++ ) {

//                                 $parametrosPresencaTrans = array( 'diaid' => $componentesCurricularesTrans[$i]['diaid']
//                                     , 'estid' => $estudantestransferidos['estid'] );
                                    
//                                 // Recupera a presenca por diário de frequencia 
//                                 $dadosPresencaTrans = listaPresencaPorAlunoTransferido( $parametrosPresencaTrans );
// //                     ver($dadosPresencaTrans,d);           
//                                 // Habilita/Desabilita campo de edição de presença
//                                 $habilitaPresenca = (!empty( $componentesCurricularesTrans[$i]['diaid'] ) ? 'N' : 'N');
                                
//                                     $habilitaPresenca = 'N';

//                                 // Criação de parametros para o Array HTML
//                                 $frqIdDoEstudanteTrans = (!empty( $dadosPresencaTrans['frqid'] ) ? $dadosPresencaTrans['frqid'] : '0' );
//                                 $estidDoEstudanteTrans = $estudantestransferidos['estid'];
//                                 $diaidDoEstudanteTrans = (!empty( $componentesCurricularesTrans[$i]['diaid'] ) ? $componentesCurricularesTrans[$i]['diaid'] : '0' );
//                                 $qtdPresenca      = (!empty( $dadosPresencaTrans['frqqtdpresenca'] ) ? $dadosPresencaTrans['frqqtdpresenca'] : '0' );
//                                 $somaQuantidadePresenca += $qtdPresenca;

//                                 echo "<td>";
//                                 //var_dump( $dadosPresenca );
//                                 $classComCur = "class='qtdaulas_{$diaidDoEstudanteTrans}'";
//                                 $idCampo     = "qtdaulas_{$diaidDoEstudanteTrans}_{$estidDoEstudanteTrans}_{$frqIdDoEstudanteTrans}";
// //                                 $vlr = "qtdaulas[" . $diaidDoEstudanteTrans . "][" . $estidDoEstudanteTrans . "][" . $frqIdDoEstudanteTrans . "]";
//                                 echo campo_texto( "$vlr", 'N', $habilitaPresenca, 'Quantidade de aulas dadas', "2", '2', '[#]', '', "", "", "", "", "", $qtdPresenca );
//                                 echo "</td>";

//                                 //$idFrequenciaEstudante = '';
//                             }
//                             echo "<td>{$somaQuantidadePresenca}</td>";
//                             echo "</tr>";
//                             $numeroLista++;
//                              // Lista os estudantes que foram transferidos
                        
//                         }
//                     }
//                     ?>
                </table>
                <input type="hidden" name="qtd_aula_prevista"  id="qtd_aula_prevista" value="<?php echo $infoDiario['soma_qtdaulaprevista']; ?>" />
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
        if(!in_array( PFL_CONSULTA, $perfis )){ 
        	if($habilitado == 'S'){?>
                <input type="button" name="salvarDiarioFrequenciaMensal" id="salvarDiarioFrequenciaMensal" value="Salvar" />
                <input type="button" name="btnFecharTrabalho" id="btnFecharTrabalho" value="Fechar Diários" />
        <?php }
		} ?>
    </div>
</div>