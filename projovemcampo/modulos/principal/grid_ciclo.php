<?php
$turId  = (int) $_REQUEST['turid'];
$ppuId  = (int) $_REQUEST['ppuid'];
$perId  = (int) $_REQUEST['perid'];

if ( !empty ($perId) && $perId != 0) {
    
    $perId = "AND per.perid  = {$perId}";
}

else {
    
    $perId = '';
    
}

$sqlGridDiarioPeriodo = "SELECT  DISTINCT
							dia.diaid,
							per.perdescricao, 
							per.perid, 
							aluno_turma_periodo.qtd_aluno_turma ,
							COALESCE(periodo_componente_frequencia.stddesc, 'Não Gerado' ) as stddesc_frequencia
						FROM projovemcampo.diario dia
						INNER JOIN projovemcampo.periodo per ON per.perid = dia.perid
						INNER JOIN projovemcampo.historico_diario hid ON hid.diaid = dia.diaid
						INNER JOIN (SELECT DISTINCT
								dia.turid, count(estid) as qtd_aluno_turma, per.perid
							    FROM
									projovemcampo.periodo per 
								INNER JOIN projovemcampo.diario dia ON per.perid = dia.perid
								INNER JOIN projovemcampo.estudante est ON est.turid = dia.turid
							    WHERE
								dia.turid = {$turId}
							    GROUP BY
								dia.turid,per.perid)as aluno_turma_periodo ON per.perid = aluno_turma_periodo.perid
						INNER JOIN (
								SELECT  DISTINCT
									dia.diaid,
									max(dia.hidid),
									hid.stdid,
									stddesc
								FROM projovemcampo.diario dia
								INNER JOIN projovemcampo.periodo per ON per.perid = dia.perid
								INNER JOIN projovemcampo.historico_diario hid ON hid.hidid = dia.hidid
								INNER JOIN projovemcampo.status_diario std ON std.stdid = hid.stdid
								WHERE
									dia.turid = {$turId}
								GROUP BY
									dia.diaid,
									hid.stdid,
									stddesc) as periodo_componente_frequencia ON dia.diaid = periodo_componente_frequencia.diaid
						 WHERE
							per.perstatus 	= 'A'
						AND 	dia.turid = {$turId}
						$perId
						ORDER BY per.perid ";
                               
                                
// ver($sqlGridDiarioPeriodo);
                                
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
$materias= array(0 => array("codigo" => "1", "descricao" => "Ciências Agrárias"),
				 1 => array("codigo" => "2", "descricao" => "Ciências Humanas"),
				 2 => array("codigo" => "3", "descricao" => "Ciências da Natureza e Matemática"),
				 3 => array("codigo" => "4", "descricao" => "Linguagem, Código e suas tecnologias"));
if(!$arrDiarioPeriodo){
?>
	<p class="error">Ainda não existe diário gerado para esse componente curricular.</p>
<? 
}else{
?>
<table border="1" cellspacing="0" cellpading="2" style="width: 100%; color: 333333;" class="listagem" id="grid_ciclo">
    <thead>
        <tr>
            <th class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Mês</th>
            <th class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Qtd. Alunos</th>
            <th class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Área de Conhecimento</th>
            <th class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Situação Diário de Frequência</th>
            <th class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ação</th>
        </tr>
    </thead>
    <tbody>
<?
	foreach( $materias as $materia  ){
?>
        
        <?php
        $cicloId    = '';
        $unidadeId  = '';
        
            
		foreach( $arrDiarioPeriodo as $diarioPeriodo  ){
			echo '<tr>';

			if( empty($diarioPeriodo['perdescricao']) ){
				 $diarioPeriodo['perdescricao'] = '&nbsp';
			}

			if( empty($diarioPeriodo['qtd_aluno_turma']) ){
				$diarioPeriodo['qtd_aluno_turma'] = '&nbsp';
			}
			if( empty($materia['descricao']) ){
				$diarioPeriodo['descricao'] = '&nbsp';
			}
			if( empty($diarioPeriodo['stddesc']) ){
				$diarioPeriodo['stddesc'] = '&nbsp';
			}

			$colunas =  '<tr><td class="textoCentro">'. $diarioPeriodo['perdescricao']  .'</td>'
                    	.'<td class="textoCentro">'. $diarioPeriodo['qtd_aluno_turma'] .'</td>'
                    	.'<td class="textoCentro">'. $materia['descricao'] .'</td>'
                    	.'<td class="textoCentro">'. $diarioPeriodo['stddesc_frequencia'] .'</td>'                  
                    	.'<td class="textoCentro">';

			if( !empty( $diarioPeriodo['diaid'] ) ){
				$colunas .= '<img src="../imagens/folder_user.png" alt="Diário de Frequência" title="Diário de Frequência Tempo Escola" class="visualizarDiarioGrid" tipoensino = "E" id="'. $diarioPeriodo['diaid'] .'" idMateria="'. $materia['codigo'] .'">
							&nbsp;
							<img src="../imagens/folder_user.png" alt="Diário de Frequência" title="Diário de Frequência Tempo Comunidade" class="visualizarDiarioGrid" tipoensino = "C" id="'. $diarioPeriodo['diaid'] .'" idMateria="'. $materia['codigo'] .'">';
			}


			$colunas .= '&nbsp</td></tr>';
			
			echo $colunas;

			echo '</tr>';
		}
        
	}
        ?>
    </tbody>
</table>
<?php 
}
?>