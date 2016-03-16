<?php
require_once APPRAIZ . 'www/projovemcampo/_funcoes.php';

$diaId  = (int) $_REQUEST['diaId'];
$idMateria  = (int) $_REQUEST['idMateria'];
$tipoensino = $_REQUEST['tipoensino'];
$sqlDadosDiario = "SELECT 
						dia.diatempoescola,
						dia.diatempocomunidade,
						tur.turdescricao, 
						ent.entid, 
						ent.entnome, 
						TO_CHAR( rap.datainicio, 'DD/MM/YYYY') as perdtinicio, 
						TO_CHAR( rap.datafim, 'DD/MM/YYYY') as perdtfim, 
						ende.endlog, 
						ende.endbai, 
						ende.estuf, 
						ende.endnum, 
						mun.mundescricao
					FROM projovemcampo.diario dia
					INNER JOIN projovemcampo.periodo per ON per.perid = dia.perid
					INNER JOIN projovemcampo.rangeperiodo rap ON dia.rapid =rap.rapid
					INNER JOIN projovemcampo.turma tur ON tur.turid = dia.turid
					INNER JOIN entidade.entidade ent ON ent.entid = tur.entid
					INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod                    
					WHERE 
						dia.diaid = {$diaId}
	           ";
// ver($sqlDadosDiario);
$sqAlunoTurma = "SELECT DISTINCT
					est.estid, 
					est.estnome
				FROM projovemcampo.diario dia
				INNER JOIN projovemcampo.turma tur ON dia.turid = tur.turid
				INNER JOIN projovemcampo.estudante est ON tur.turid = est.turid
                WHERE 
					dia.diaid = {$diaId}
                AND est.eststatus = 'A' 
                ORDER BY est.estnome";

$arrAlunosTurma = $db->carregar($sqAlunoTurma);
$dadosDiario     = $db->pegaLinha($sqlDadosDiario);
$sql = "SELECT 
			'1' as codigo, 
			mun.mundescricao, 
			mun.estuf, 
			mun.muncod
		FROM projovemcampo.turma tur
		INNER JOIN entidade.entidade ent ON ent.entid = tur.entid
		INNER JOIN projovemcampo.diario dia ON tur.turid = dia.turid
		INNER JOIN territorios.municipio mun ON mun.muncod = mun.muncod 
        WHERE
			dia.diaid       = {$diaId}";
$dado = $db->pegaLinha($sql);
    

$sqlDadosInstituicao = "SELECT DISTINCT 
							ee.entnome, 
							ee.entcodent
						FROM projovemcampo.usuarioresponsabilidade ur
						INNER JOIN  projovemcampo.turma tur ON tur.entid = ur.entid and tur.turstatus = 'A'
						INNER JOIN  entidade.entidade ee ON ee.entid = ur.entid
						WHERE
							ur.rpustatus='A'
                        AND ur.usucpf = '{$_SESSION['usucpf']}'";

$dadosInstituicao       = $db->pegaLinha($sqlDadosInstituicao);

if($idMateria == 1){
	$materia='Ciências Agrárias';
}elseif($idMateria == 2){
	$materia='Ciências Humanas';
}elseif($idMateria == 3){
	$materia='Ciências da Natureza e Matemática';
}elseif($idMateria == 4){
	$materia='Linguagem, Código e suas tecnologias';
}
if($tipoensino == 'E'){
	$tipodiario = 'Tempo Escola';
}else{
	$tipodiario = 'Tempo Comunidade';
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
    
    .mes{
        font-size: 15px; 
        font-weight: bold;
    }
</style>
<link rel="stylesheet" media="print" type="text/css" href="../projovemcampo/css/print_diario.css" />
<div>
    <table id="tableDiarioFrequencia" style="width: 100%;" border="0" >
        <tr>
            <td colspan="5">
                <!-- Cabeçalho -->
                <table id="cabecalho" border="0" style="width: 100%; border: 1px solid black;">
                    <tr>
                        <td rowspan="4" style="width: 150px;" > <img  src="../imagens/projovemcampo_cinza.jpg" alt="PROJOVEM CAMPO" title="projovemcampo" style="font-size: 15px;"/> </td>
                        <td class="textoCentro"> Ministério da Educação </td>
                    </tr>
                    <tr>
                        <td class="textoCentro"> Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão </td>
                    </tr>
                    <tr>
                        <td class="textoCentro">DIÁRIO DE FREQUÊNCIA</td>
                    </tr>
                    <tr>
                        <td class="textoCentro" style="font-size: 20px;"><strong>Componente Curricular: <?php echo $materia ?></strong></td>
                    </tr>
                </table>
            </td>
        </tr>
        
        
        <!-- Informacoes -->
        <tbody>
            <tr>
                <td class="colunaLabel">Coordenação Distrital/Estadual/Municipal:</td>
                <td style="width: 800px;" colspan="2"><?php echo montaTituloEstMun(); ?></td>
                <td class="colunaLabel">AULAS DADAS</td>
                <td> </td>
            </tr>
            <tr>
                <td class="colunaLabel">Escola:</td>
                <td colspan="2"><?php echo 'Escola: ', $dadosInstituicao['entnome'], ' - Escola: ', $dadosDiario['entid']; ?></td>
                <td class="colunaLabel"><? echo $tipodiario ?>: ______</td>
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
                <td style="width: 200px;"><?php echo $dadosDiario['turdescricao']; ?></td>
                <td class="mes" colspan="3">Mês: <?php echo $dadosDiario['perdtinicio'], ' a ', $dadosDiario['perdtfim']; ?></td>
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
                                echo "<td>{$arrAlunosTurma[$ct]['estid']}</td>";
                                echo "<td class=\"nomeAluno\">{$arrAlunosTurma[$ct]['estnome']}</td>";

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
        <input type="hidden" name="diario_id" id="diario_id" value="<?php echo $diaId; ?>" />
        <input type="hidden" name="materia_id" id="materia_id" value="<?php echo $idMateria; ?>" />
        <input type="hidden" name="tipoensino" id="tipoensino" value="<?php echo $tipoensino; ?>" />
    </div>
</div>
<script type="text/javascript">
    
    if( typeof($) == 'function' ){
        $(document).ready(function(){
            $('#btnImprimirDiario').click(function(){
                var diaid = $('#diario_id').val();
                var materia = $('#materia_id').val();
                var tipoensino = $('#tipoensino').val();
                window.open( 'projovemcampo.php?modulo=principal/monitoramento/diarioImpresso&acao=A&diaId='+ diaid+'&idMateria='+materia+'&tipoensino='+tipoensino, 'teste' );
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