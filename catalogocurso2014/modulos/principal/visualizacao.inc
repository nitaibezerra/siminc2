<?php
if($_REQUEST['req']!=''){
    $_REQUEST['req']($_REQUEST);
}

if( $_SESSION['catalogo']['curid'] ){
    $permissoes = testaPermissao();
} else {
    $permissoes['gravar'] = true;
}

$dadosContato = recuperaContato($_REQUEST);

if(is_array($dadosContato)){
    foreach($dadosContato as $k => $dado){
        $$k = $dado;
    }
}
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

monta_abas(); ?>

<script src="/includes/JQuery/jquery-1.9.1/jquery-1.9.1.js" type="text/javascript"></script>
<script language="javascript" type="text/javascript">
	$( document ).ready(function() {
		var exercicio = "<h2 style=\"text-align: center;\">Catálogo Curso "+<?php echo $_SESSION['exercicio']; ?>+"</h2>";
	    jQuery('#top_content_label_sistema').html(exercicio);
	});
</script>
<div class="row">
    <div class="col-md-12">
        <ul class="breadcrumb">
            <li>
                <strong>Curso: </strong>
                <?php echo $_SESSION['catalogo']['curid'] .' - ' . $_SESSION['catalogo']['curdesc']; ?>
            </li>
        </ul>
    </div>
</div>
<?php
if($_SESSION['catalogo']['curid']){
   $curid = $_SESSION['catalogo']['curid'];
   $_REQUEST['curid'] = $_SESSION['catalogo']['curid'];
}

if($_REQUEST['req'] != ''){
    $_REQUEST['req']($_REQUEST);
}

$dadosCurso = recuperaDadosCurso($_REQUEST);

monta_titulo('Dados Gerais', ''); ?>
 
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td>
        <?php if($_SESSION['catalogo']['curid']){
        	montaCabecalho($_SESSION['catalogo']['curid']);
        } ?>
        </td>
    </tr>
</table>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubTituloDireita">Código / Nome do Curso</td>
        <td><?php echo $dadosCurso['curid']; ?> / <?php echo $dadosCurso['curdesc']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="15%">Status</td>
        <td><?php echo $dadosCurso['curstatus']=='A'?'Ativo':'Inativo'; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="15%">Código - Área</td>
        <td>
            <?php if($dadosCurso['ateid']){ 
            $sql = "SELECT    ateid||' / '||atedesc AS descricao
            	    FROM	  catalogocurso2014.areatematica
                	WHERE     atestatus = 'A' AND ateid = {$dadosCurso['ateid']} AND ateano = {$_SESSION['exercicio']}";
            
            echo $db->pegaUm($sql); 
            } ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="15%">Subárea</td>
        <td>
            <?php if($dadosCurso['suaid']){ 
            $sql = "SELECT 	suadesc as descricao
               	    FROM 	catalogocurso2014.subarea
                    WHERE 	suaid = '{$dadosCurso['suaid']}' AND subano = {$_SESSION['exercicio']}"; 
            echo $db->pegaUm($sql); 
            } ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="15%">Especialidade</td>
        <td>
            <?php
            if($dadosCurso['espid']){

            $sql = "SELECT espdesc AS descricao
                    FROM catalogocurso2014.especialidade 
                    WHERE espid = '{$dadosCurso['espid']}' AND espano = {$_SESSION['exercicio']}";
            
            echo $db->pegaUm($sql); 
            } ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Nivel do Curso</td>
        <td>
        	<?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2014){
        		$ncuid = $dadosCurso['ncuid'];
        	} else {
        		if(is_array($dadosCurso['ncuid']) && !empty($dadosCurso['ncuid'])){
        			$ncuid = implode(",",$dadosCurso['ncuid']);
        		} else {
        			$ncuid = 0;
        		}
        	}
        	
        	$sql = "SELECT ncudesc FROM catalogocurso2014.nivelcurso WHERE ncuano = {$_SESSION['exercicio']} AND ncuid IN ({$ncuid})"; 
            $dados = $db->carregarColuna($sql);
            
            foreach($dados as $dado){
            	echo $dado."<br>";
            } ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="15%">Rede</td>
        <td>
            <?php
            if( is_array($dadosCurso['redid']) ){
            	$redid = implode(',',$dadosCurso['redid']);
            	$sql = "SELECT reddesc FROM catalogocurso2014.rede WHERE redano = {$_SESSION['exercicio']} AND redid IN ({$redid})";
                $dados = $db->carregarColuna($sql);
                foreach($dados as $dado){
                    echo $dado."<br>";
                }
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Objetivo</td>
        <td><?php echo $dadosCurso['curobjetivo']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Descrição do Curso</td>
        <td><?php echo $dadosCurso['curementa']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="15%">Metodologia</td>
        <td><?php echo $dadosCurso['curmetodologia']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Modalidade de Ensino</td>
        <td>
            <?php
            $dadosCurso['modid'] = is_array($dadosCurso['modid']) ? $dadosCurso['modid'] : Array();
            $modid = implode(',',$dadosCurso['modid']);
            $sql = "SELECT modid AS codigo, moddesc AS descricao FROM catalogocurso2014.modalidadecurso WHERE modano = {$_SESSION['exercicio']} AND modid IN ({$modid}) AND modstatus = 'A'";
            $modalidades = $db->carregar($sql);
            $modids = Array();
            foreach($modalidades as $k => $modalidade){
                echo "<br> - ".$modalidade['descricao'];
                $modids[$k] = $modalidade['codigo'];
            }
            echo "<br>	"; ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Carga Horária Presencial</td>
        <td><?php if($dadosCurso['curchmim']) echo $dadosCurso['curchmim'] . ' Horas.'; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Carga Horária Distância</td>
        <td><?php if($dadosCurso['curchmax']) echo $dadosCurso['curchmax'] . ' Horas.'; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Número de alunos por<br> turma</td>
        <td><?php echo $dadosCurso['curnumestudanteidealpre']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Periodicidade de Monitoramento</td>
        <td><?php if($dadosCurso['uteid']){
        		$sql = "SELECT utedesc FROM catalogocurso2014.unidadetempo WHERE uteano = {$_SESSION['exercicio']} AND uteid = {$dadosCurso['uteid']}";
        		echo $db->pegaUm($sql); 
        	}?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Infraestrutura Recomendada</td>
        <td><?php echo $dadosCurso['curinfra']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Tempo de duração</td>
        <td><?php echo $dadosCurso['curduracao']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Responsável pela ultima alteração</td>
        <td>
            <?php echo $dadosCurso['usunome']; ?> |
            <?php echo $dadosCurso['hicdata']; ?>
        </td>
    </tr>
</table>
<?php 
monta_titulo('Organização do Curso' , ''); 

global $db,$permissoes;

$sql = "SELECT		tiodesc,
					orcdesc,
					coalesce(orcchmim,0) AS mim,
					orcementa
		FROM		catalogocurso2014.organizacaocurso orc
		LEFT JOIN 	catalogocurso2014.tipoorganizacao tor ON tor.tioid = orc.tioid
		LEFT JOIN 	catalogocurso2014.modalidadecurso mod ON mod.modid = orc.modid
		WHERE		orcstatus = 'A' AND orc.curid = {$_SESSION['catalogo']['curid']}";

$cursos = $db->carregar($sql);
$cabecalho = array("Tipo", "Nome", "Carga Horária", "Descrição");
$db->monta_lista_array($cursos, $cabecalho, 50, 20, 'INT', '100%', '',$arrayDeTiposParaOrdenacao); ?>

<br>

<?php monta_titulo('Equipe' , ''); ?>
<?php listaEquipes($_POST, false); ?>

<br>

<?php monta_titulo('Público-Alvo' , ''); ?>
<?php $dadosPublico = recuperaDadosPublicoAlvo($_REQUEST); ?>
<?php listarPerfil($curid,'visualizar'); ?>

<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubTituloDireita">Outras Exigências:</td>
        <td><?php echo $dadosPublico['curpaoutrasexig']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Curso disponivel para demanda social?:</td>
        <td><?php echo $dadosPublico['curpademsocial']=='t'?'Sim':'Não'; ?></td>
    </tr>
    <?php if( $dadosPublico['curpademsocial']=='t' ){?>
        <tr class="tr_demsoc">
            <td class="SubTituloDireita">Percentual máximo de participantes na demanda social:</td>
            <td>
                <?php echo $dadosPublico['curpademsocialpercmax']; ?>
                %
                <img align="middle" border="0" src="../imagens/obrig.gif" style="margin-bottom:7px">
            </td>
        </tr>
        <tr class="tr_demsoc">
            <td class="SubTituloDireita">
                Público-alvo da demanda social&nbsp;
            </td>
            <td>
                <?php
                if( is_array($dadosPublico['padid']) && count($dadosPublico['padid']) ){
                    $dados = $dadosPublico['padid'];
                    if( is_array($dados) ){
                        foreach($dados as $dado){
                            echo $dado['descricao']."<br>";
                        }
                    }
                }
                ?>
            </td>
        </tr>
    <?php }?>
</table>
<br />
<?php  monta_titulo('IES Ofertante' , ''); ?>

<?php listarIESOfertante($curid); ?>

<br>

<?php monta_titulo('Contato no MEC ou Capes' , ''); ?>
<?php $dadosContato = recuperaContato($_REQUEST); ?>

<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubTituloDireita" width="15%"><?php echo ($_SESSION['exercicio'] == ANO_EXERCICIO_2014) ? 'Coordenação responsável no<br> MEC ou na CAPES:' : 'Diretoria Responsável:'; ?></td>
        <td>
            <?php
            if($dadosCurso['coordid']){
                $sql = "SELECT     coordsigla||' - '||coorddesc AS descricao
                    	FROM       catalogocurso2014.coordenacao
                    	WHERE      coordstatus = 'A' AND coordid = {$dadosCurso['coordid']} AND coorano = {$_SESSION['exercicio']}";

                $coordenacao = $db->pegaUm($sql);
                echo $coordenacao;
            } ?>
        </td>
    </tr>
    <?php if($_SESSION['exercicio']==ANO_EXERCICIO_2015){ ?>
    <tr>
        <td class="SubTituloDireita" width="15%">Coordenação Responsável</td>
        <td>
        	<?php 
            if($dadosCurso['corid']){
				$sql = "SELECT 	cordesc AS descricao
						FROM 	catalogocurso2014.coordresponsavel
			  			WHERE 	corid = {$dadosCurso['corid']} AND corstatus = 'A' AND corano = {$_SESSION['exercicio']}";             	
                
				$coordenacaoresp = $db->pegaUm($sql);
                echo $coordenacaoresp;
            } ?>
        </td>
    </tr>
    <?php } ?>
    <tr>
        <td class="SubTituloDireita" width="15%">Telefone Principal:</td>
        <td><?php echo $dadosContato['curconttel']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Telefone Adicional:</td>
        <td><?php echo $dadosContato['curconttel2']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Nome:</td>
        <td><?php echo $dadosContato['curcontdesc']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">E-mail:</td>
        <td><?php echo $dadosContato['curcontemail']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Site:</td>
        <td><?php echo $dadosContato['curcontsite']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Outras Informações:</td>
        <td><?php echo $dadosContato['curcontinfo']; ?></td>
    </tr>
	<tr>
        <td colspan="2" class="FundoTitulo" style="text-align: center;">
            <input  type="button" value="Voltar" onclick="javascript:window.location.href = '/catalogocurso2014/catalogocurso2014.php?modulo=principal/cadAbrangencia&acao=A'" />
            <input type="button" value="Imprimir" onclick=self:print()>
        </td>
    </tr>
</table>
