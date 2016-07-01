<?php $_SESSION['catalogo']['curid'] = $_REQUEST['curid']; ?>
<html>
<head>
	<title>Catálogo de Curso</title>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
</head>
<body>
<?php
if( $_SESSION['catalogo']['curid'] ){
	$curid = $_SESSION['catalogo']['curid'];
    $_REQUEST['curid'] = $_SESSION['catalogo']['curid'];
}
    
if($_REQUEST['req']!=''){
	$_REQUEST['req']($_REQUEST);
}

$dadosCurso = recuperaDadosCurso($_REQUEST); ?>
    
<?php monta_titulo('Dados Gerais' , ''); ?>
    
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
               	 $sql = "SELECT 	 ateid||' / '||atedesc as descricao
               			 FROM        catalogocurso2014.areatematica
               			 WHERE       atestatus = 'A' AND ateid = {$dadosCurso['ateid']} AND ateano = {$_SESSION['exercicio']}";
                	 
               	 echo $db->pegaUm($sql);
	        } ?>
	   	</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="15%">Subárea</td>
	    <td>
	    	<?php if($dadosCurso['suaid']){
	            $sql = "SELECT 	suadesc AS descricao
	                    FROM 	catalogocurso2014.subarea
	                    WHERE 	suaid = '{$dadosCurso['suaid']}' AND subano = {$_SESSION['exercicio']}";
	            
	            echo $db->pegaUm($sql); 
	        } ?>
	    </td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="15%">Especialidade</td>
	    <td>
	    	<?php if($dadosCurso['espid']){
	            $sql = "SELECT 	espdesc AS descricao
	                   	FROM 	catalogocurso2014.especialidade
	                   	WHERE 	espid = '{$dadosCurso['espid']}' AND espano = {$_SESSION['exercicio']}"; 
	             
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
	        } ?>
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
	    <td><?php if($dadosCurso['curchmim']){
	   		 	echo $dadosCurso['curchmim'] . ' Horas.';
	        } ?>
	    </td>
	</tr>
	<tr>
	    <td class="SubTituloDireita">Carga Horária distancia</td>
	    <td><?php if($dadosCurso['curchmax']){
	        	echo $dadosCurso['curchmax'] . ' Horas.';
	        }?>
	    </td>
	</tr>
	<tr>
	    <td class="SubTituloDireita">Número de alunos por<br> turma</td>
	    <td><?php echo $dadosCurso['curnumestudanteidealpre']; ?>.</td>
	</tr>
	<tr>
	   	<td class="SubTituloDireita">Periodicidade de Monitoramento</td>
	    <td>
	    	<?php echo $dadosCurso['curqtdmonitora']; ?>
	        <?php if($dadosCurso['uteid']) 
	        $sql = "SELECT utedesc FROM catalogocurso2014.unidadetempo WHERE uteano = {$_SESSION['exercicio']} AND uteid = {$dadosCurso['uteid']}";
	        echo $db->pegaUm($sql); ?>
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

<?php monta_titulo('Organização do Curso' , ''); ?>

<?php
global $db,$permissoes;

$sql = "SELECT			tiodesc,
						orcdesc,
						coalesce(orcchmim,0) as mim,
						orcementa
		FROM			catalogocurso2014.organizacaocurso orc
		LEFT JOIN 		catalogocurso2014.tipoorganizacao tor ON tor.tioid = orc.tioid
		LEFT JOIN 		catalogocurso2014.modalidadecurso mod ON mod.modid = orc.modid
		WHERE			orcstatus = 'A'	AND orc.curid = {$_SESSION['catalogo']['curid']}";
    
$cursos = $db->carregar($sql);
$cabecalho = array("Tipo", "Nome", "Carga Horária", "Descrição");
$db->monta_lista_array($cursos, $cabecalho, 50, 20, 'INT', '100%', '',$arrayDeTiposParaOrdenacao); ?>
    
<br>
<?php monta_titulo('Equipe' , ''); ?>
<?php listaEquipes($_POST, false); ?>

<br>
<?php monta_titulo('Público-Alvo' , ''); ?>
<?php $dadosPublico = recuperaDadosPublicoAlvo($_REQUEST); ?>

<table class="tabela listagem" align="center" border="0" cellpadding="5" cellspacing="1">
	<thead>
		<tr>
	    	<td>Perfil</td>
	        <td>Area de formação</td>
	        <td>Disciplina</td>
	        <td>Etapa de ensino</td>
	        <td>Modo de ensino</td>
	        <td>Nivel escolaridade</td>
		</tr>
	</thead>
    <tbody id="container_lista">
    <?php
    $sqlListaPerfil = "SELECT			paf.pafid, 
										fe.fexid, 
										fexdesc,
										-- Area
										array_to_string(array(
												SELECT	 	no_nome_area_ocde
												FROM 		catalogocurso2014.publicoalvo_assocareaformacao a
												INNER JOIN 	educacenso_".(ANO_CENSO).".tab_area_ocde af ON af.pk_cod_area_ocde = a.pk_cod_area_ocde
												WHERE 		a.pafid = paf.pafid AND a.pk_cod_area_ocde_ano = {$_SESSION['exercicio']}
											), '<br /> ') AS area_formacao,
										-- Disciplina
										array_to_string(array(
												SELECT 		no_disciplina
												FROM 		catalogocurso2014.publicoalvo_assocdisciplina pad
												INNER JOIN 	educacenso_".(ANO_CENSO).".tab_disciplina d ON d.pk_cod_disciplina = pad.pk_cod_disciplina
												WHERE 		pad.pafid = paf.pafid AND pad.asdano = {$_SESSION['exercicio']}
											), '<br /> ') AS disciplina,
										-- Etapa de ensino
										array_to_string(array(
												SELECT 		no_etapa_ensino
												FROM 		catalogocurso2014.publicoalvo_assocetapaensino e
												INNER JOIN 	educacenso_".(ANO_CENSO).".tab_etapa_ensino ee ON ee.pk_cod_etapa_ensino = e.pk_cod_etapa_ensino
												WHERE 		e.pafid = paf.pafid AND e.aseano = {$_SESSION['exercicio']}
											), '<br /> ') AS etapa_ensino,		 								
										-- Mod Ensino
										array_to_string(array(
												SELECT 		no_mod_ensino
												FROM 		catalogocurso2014.publicoalvo_assocmodensino pme
												INNER JOIN 	educacenso_".(ANO_CENSO).".tab_mod_ensino me ON me.pk_cod_mod_ensino = pme.modensinoid
												WHERE 		pme.pafid = paf.pafid AND pme.amoano = {$_SESSION['exercicio']}
											), '<br /> ') AS mod_ensino,										
										-- Nivel Escolaridade
										array_to_string(array(
												SELECT 		no_escolaridade
												FROM 		catalogocurso2014.publicoalvo_assocnivelescolaridade pne
												INNER JOIN 	educacenso_".(ANO_CENSO).".tab_escolaridade ne ON ne.pk_cod_escolaridade = pne.nivelescolaridadeid
												WHERE 		pne.pafid = paf.pafid AND pne.aniano = {$_SESSION['exercicio']}
											), '<br /> ') AS nivel_escolaridade
						FROM 			catalogocurso2014.publicoalvo_assocfuncaoexercida paf
						LEFT JOIN 		catalogocurso2014.funcaoexercida fe ON fe.fexid = paf.fexid
						WHERE 			curid = {$curid} AND fexstatus = 'A'";
    
	$listaPerfil = $db->carregar($sqlListaPerfil); ?>
    <?php $idRand = (double)microtime()*1000000;?>
    <?php if($listaPerfil){ ?>
    <?php foreach ( $listaPerfil as $perfil){ ?>
	    <tr class="linha_listagem" id="ies_<?php echo $idRand ?>" style="background-color: #fafafa;">
    	    <!-- Perfil -->
            <td><?php echo $perfil['fexdesc']; ?></td>
            <!-- Area de formação -->
            <td><?php echo $perfil['area_formacao']; ?></td>
            <!-- Disciplina -->
            <td><?php echo $perfil['disciplina']; ?></td>
            <!-- Etapa de ensin -->
            <td><?php echo $perfil['etapa_ensino']; ?></td>
            <!-- Mod Ensino -->
            <td><?php echo $perfil['mod_ensino']; ?></td>
            <!--  Nivel Escolaridade -->
            <td><?php echo $perfil['nivel_escolaridade']; ?></td>
        </tr>
        <?php } ?>
    <?php } ?>
    </tbody>
</table>

<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
    	<td class="SubTituloDireita">Outras Exigências&nbsp;</td>
        <td><?php echo $dadosPublico['curpaoutrasexig']; ?></td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Curso disponivel para demanda social?&nbsp;</td>
        <td><?php echo $dadosPublico['curpademsocial']=='t'?'Sim':'Não' ?></td>
    </tr>
    <?php if( $dadosPublico['curpademsocial']=='t' ){?>
    <tr class="tr_demsoc">
    	<td class="SubTituloDireita">Percentual máximo de participantes na demanda social&nbsp;</td>
        <td>
        	<?php echo $dadosPublico['curpademsocialpercmax']; ?>
            %
            <img align="middle" border="0" src="../imagens/obrig.gif" style="margin-bottom:7px">
        </td>
	</tr>
    <tr class="tr_demsoc">
    	<td class="SubTituloDireita">Público-alvo da demanda social&nbsp;</td>
        <td>
        	<?php
            if( is_array($dadosPublico['padid']) && count($dadosPublico['padid']) ){
            	$dados = $dadosPublico['padid'];
                if( is_array($dados) ){
                	foreach($dados as $dado){
                    	echo $dado['descricao']."<br>";
                    }
                }
            } ?>
		</td>
	</tr>
    <?php } ?>
</table>

<br />

<?php monta_titulo('IES Ofertante' , ''); ?>

<?php
// IES Ofertante.
// Recuperar dados IES Ofertante.
$sqlIESOfertante = "SELECT 			ieo.ieoid, 
  									ies.entid, 
   									ieo.unicod, 
   									ies.entsig, 
   									ies.entnome, 
   									ieo.curid, 
   									cur.curdesc, 
   									cor.coordsigla, 
   									ieo.ieoanoprojeto, 
   									ieo.ieoqtdvagas
                    FROM 			catalogocurso2014.iesofertante ieo
                    INNER JOIN 		catalogocurso2014.curso cur ON cur.curid = ieo.curid
                    INNER JOIN 		entidade.entidade ies ON ies.entid = ieo.entid
                    LEFT JOIN 		catalogocurso2014.coordenacao cor ON cor.coordid  = cur.coordid
                   	WHERE 			ieo.curid = {$curid} AND ieo.ieostatus = 'A' AND cur.curano = {$_SESSION['exercicio']}
                    ORDER BY 		ies.entsig , ies.entnome";
    
$iesOfertante = $db->carregar($sqlIESOfertante); ?>

<table class="tabela listagem" align="center" border="0" cellpadding="5" cellspacing="1">
	<thead>
        <tr>
            <td>Código UO</td>
            <td>Sigla IES</td>
            <td>Nome IES</td>
            <td>Ano</td>
            <td>Qtd. Vagas</td>
        </tr>
    </thead>
    <tbody id="container_list_ies">
    <?php if($iesOfertante){ ?>
    <?php foreach($iesOfertante as $ieOfertante){ ?>
	<?php $idRand = (double)microtime()*1000000;?>
    	<tr class="linha_listagem" id="ies_<?php echo $idRand ?>" style="background-color: #fafafa;">
	        <input type="hidden" name="ieoid[]" value="<?php echo $ieOfertante['ieoid']; ?>">
            <input type="hidden" name="ies[]" value="<?php echo $ieOfertante['entid']; ?>">
            <input type="hidden" name="uo[]" value="<?php echo $ieOfertante['unicod']; ?>">
            <input type="hidden" name="nome_ies[]" value="<?php echo $ieOfertante['entnome']; ?>">
            <!-- Ação -->
            <!-- Codigo UO -->
            <td><?php echo $ieOfertante['unicod']; ?></td>
            <!-- Abreviação IES -->
            <td><?php echo $ieOfertante['entsig']; ?></td>
            <!-- Nome IES -->
            <td><?php echo $ieOfertante['entnome']; ?></td>
            <!-- Ano -->
            <td><?php echo $ieOfertante['ieoanoprojeto']; ?></td>
            <!-- Qtd Vagas -->
            <td><?php echo $ieOfertante['ieoqtdvagas']; ?></td>
		</tr>
        <?php } ?>  
	<?php } ?>
    </tbody>
</table>

<br>
    
<?php monta_titulo('Contato no MEC ou Capes' , ''); ?>

<?php $dadosContato = recuperaContato($_REQUEST); ?>
    
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
    	<td class="SubTituloDireita" width="15%">Coordenação responsável no MEC ou na CAPES</td>
        <td>
        <?php
        if($dadosCurso['coordid']){
        	$sql = "SELECT   coordsigla||' - '||coorddesc as descricao
               		FROM     catalogocurso2014.coordenacao
               		WHERE    coordstatus = 'A' AND coordid = {$dadosCurso['coordid']} AND coorano = {$_SESSION['exercicio']}";

            $coordenacao = $db->pegaUm($sql);
            echo $coordenacao;
        } ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="15%">Telefone Principal&nbsp;</td>
        <td><?php echo $dadosContato['curconttel']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Telefone Adicional&nbsp;</td>
        <td><?php echo $dadosContato['curconttel2']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Nome&nbsp;</td>
        <td><?php echo $dadosContato['curcontdesc']; ?></td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">E-mail&nbsp;</td>
        <td><?php echo $dadosContato['curcontemail']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Site&nbsp;</td>
        <td><?php echo $dadosContato['curcontsite']; ?></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Outras Informações&nbsp;</td>
        <td><?php echo $dadosContato['curcontinfo']; ?></td>
    </tr>
    <tr>
    	<td colspan="2" class="FundoTitulo" style="text-align: center;">
        	<input type="button" value="Imprimir" onclick="self:print();">
        </td>
	</tr>
</table>
