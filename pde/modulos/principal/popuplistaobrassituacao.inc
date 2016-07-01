<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
// Educação Básica Obras 1
$_SESSION['obras']['orgid'] = 2;
// Educação Básica Obras 2
$_SESSION['obras2']['orgid'] = 3;
?>
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=7" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />
    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
    <script language="javascript" type="text/javascript" src="js/estrategico.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
    <script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>

    <script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
    <script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

    <script language="javascript" src="/estrutura/js/funcoes.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link rel='stylesheet' type='text/css' href='css/cockpit.css'/>

    <script>
        jQuery.noConflict();

        atualizaUsuario();
    </script>

    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/fundocreche.jpg')};

        .tabela_listagem {
            background-color: #FFFFFF;
            color: #000000;
        }

        .filtro_listagem{
           width: 70%;
        }
    </style>

    <script>
    	jQuery.noConflict();
    	jQuery('.div_fotos_interno').cycle({
    		fx: 'scrollDown'
    	});

    	atualizaUsuario();

    	function abreObras(obrid){
    		window.open('/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid='+obrid);
    	}
		
		function abreObras1(obrid){
    		window.open('/obras/obras.php?modulo=principal/cadastro&acao=A&obrid='+obrid);
    	}

        function submeterFormularioListagem(){
            jQuery('#formulario_filtro_listagem').attr('action', window.location.href).submit();
        }

        jQuery(function(){

            jQuery('.ordenador_listagem').click(function(){
                jQuery('#ordenador_listagem').val(jQuery(this).attr('ordenar'));
                submeterFormularioListagem();
            });

            jQuery('.img_filtro_listagem').click(function(){
                submeterFormularioListagem();
            });

            jQuery('.filtro_listagem').keypress(function(e) {
                if(e.which == 13) {
                    submeterFormularioListagem();
                }
            });
        });
    </script>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<div style="float:right;cursor:pointer;" onclick="window.close();">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>

<form action="" method="post" name="formulario_filtro_listagem" id="formulario_filtro_listagem">
    <input type="hidden" name="ordenador_listagem" id="ordenador_listagem" value="<?php echo $_REQUEST['ordenador_listagem']; ?>"/>

    <table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">

    	<!-- Título-->
    	<tr>
    		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Lista de Obras</div></td>
    	</tr>
    	<?php

    	$order = '';

        switch ($_REQUEST['tipo']) {
            // BRASIL PROFISSIONALIZADO
            case (1):
                $sql = "SELECT  o.obrid, esd.esdid, esd.esddsc as situacao,
                                CASE
                                    WHEN o.tobid IN (3,4,5) THEN 'Ampliação / Reforma'
                                    ELSE 'Construção'
                                END AS tipo
                                , o.obrnome, o.obrdsc, ed.muncod, ed.estuf, m.mundescricao
                        FROM obras2.obras o
                            INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
                            INNER JOIN workflow.documento d ON d.docid = o.docid
                            INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
                            LEFT JOIN  entidade.endereco ed on ed.endid = o.endid
                            LEFT JOIN  territorios.municipio m on m.muncod = ed.muncod
                        WHERE o.obrstatus = 'A'
                        AND e.orgid=3
                        AND e.prfid=40 --Brasil Profissionalizado
                        AND o.obridpai is null
                        AND o.obrid NOT IN (7828,7829,7840,1000015,1000046,1000049) --Obras de teste
                        AND esd.esdid = '{$_REQUEST['situacao']}' ";

                        $order = ' order by ed.estuf, m.mundescricao, o.obrnome ';
                break;


            // REDE FEDERAL DE EPT
            case (2):
                $sql = "SELECT  s.stoid, s.stodesc as situacao,
                                CASE
                                    WHEN o.tobraid IN (3,4,5) THEN 'Ampliação / Reforma'
                                    ELSE 'Construção'
                                END AS tipo,
                                o.obrid, o.obrdesc as obrnome, ed.muncod, ed.estuf, m.mundescricao
                        FROM obras.obrainfraestrutura o
                            INNER JOIN obras.situacaoobra s ON s.stoid = o.stoid
                            LEFT JOIN  entidade.endereco ed on ed.endid = o.endid
                            LEFT JOIN  territorios.municipio m on m.muncod = ed.muncod
                        WHERE o.obsstatus = 'A'
                        AND o.orgid = 2
                        AND s.stoid = '{$_REQUEST['situacao']}' ";

                        $order = ' order by ed.estuf, m.mundescricao, o.obrdesc ';
                break;
        }

        if ($_REQUEST['ordenador_listagem']) {
            $order = " order by " . str_replace('__', ' ', $_REQUEST['ordenador_listagem']);
        }

        $sql .= $order;

        $dados = $db->carregar($sql);

        $obras = array('Ampliação / Reforma'=>array(), 'Construção'=>array());
        foreach($dados as $dado){
            if (verificarFiltroListaObrasSituacao($dado)) {
                $obras[$dado['tipo']][] = $dado;
            }
        }
    	?>
    	<tr>
    		<td class="fundo_padrao" width="50%">
    			<div>
    				<img style="float:left" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
    				<div style="float:left" class="titulo_box" >Ampliação / Reforma<br/> </div>
    			</div>
    			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <td class="center bold" >
                            Obra-ID
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="obrid" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="obrid__desc"/>
                        </td>
                        <td class="center bold" >
                            Obra
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="obrnome" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="obrnome__desc"/>
                        </td>
                        <td class="center bold" >
                            UF
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="estuf" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="estuf__desc"/>
                        </td>
                        <td class="center bold" >
                            Município
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mundescricao" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mundescricao__desc"/>
                        </td>
                    </tr>
                    <tr>
                        <th class="center" >
                            <img width="20px" class="img_filtro_listagem" src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_obrid" value="<?php echo $_REQUEST['filtro_obrid']; ?>" />
                        </th>
                        <th class="center" >
                            <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_obrnome" value="<?php echo $_REQUEST['filtro_obrnome']; ?>" />
                        </th>
                        <th class="center" >
                            <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_estuf" value="<?php echo $_REQUEST['filtro_estuf']; ?>" />
                        </th>
                        <th class="center" >
                            <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_mundescricao" value="<?php echo $_REQUEST['filtro_mundescricao']; ?>" />
                        </th>
                    </tr>
    				<?php
    				$totalObras=0;
    				if($obras['Ampliação / Reforma']){
    					foreach($obras['Ampliação / Reforma'] as $count => $dado):
    					?>
    					<tr <?php echo ($count%2) ? 'class="zebrado"' : ''; ?>>
    						<td class="" width="20%">
    							<?php
    							if($_REQUEST['tipo']==1){?>
    								<img class="link" src="cockpit/images/icone_o.png" style="vertical-align:middle;" width="20" height="20" title="Obras" onclick="abreObras(<?=$dado['obrid']?>);" />
    							<?php
    							}else{
								?>
									<img class="link" src="cockpit/images/icone_o.png" style="vertical-align:middle;" width="20" height="20" title="Obras" onclick="abreObras1(<?=$dado['obrid']?>);" />
								<?php
								}
    							?>

    							<?php echo $dado['obrid'] ?>
    						</td>
    						<td class="" width="45%"><?php echo $dado['obrnome'] ?></td>
    						<td class="center" width="15%"><?php echo $dado['estuf'] ?></td>
    						<td class="" width="20%"><?php echo $dado['mundescricao'] ?></td>
    					</tr>
    					<?php
    					$totalObras+=1;
    					endforeach;
    						?>
    						<tr>
    							<th class="bold" colspan="3">Total de Obras</th>
    							<th class="bold numero" ><?php echo $totalObras ?></th>
    						</tr>
    					<?php
    					}else{
    					?>
    						<tr>
    							<td  class="center" style="color:red" colspan="5">Não existem obras nessa situação.</td>
    						</tr>
    					<?php
    					}
    					?>
    			</table>
    		</td>
    		<td class="fundo_padrao" width="50%">
    			<div>
    				<img style="float:left" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
    				<div style="float:left" class="titulo_box" >Construção<br/> </div>
    			</div>
    			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <td class="center bold" >
                            Obra-ID
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="obrid" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="obrid__desc"/>
                        </td>
                        <td class="center bold" >
                            Obra
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="obrnome" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="obrnome__desc"/>
                        </td>
                        <td class="center bold" >
                            UF
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="estuf" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="estuf__desc"/>
                        </td>
                        <td class="center bold" >
                            Município
                            <img src="../imagens/seta_baixo.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mundescricao" />
                            <img src="../imagens/seta_cima.gif" style="vertical-align:middle;" class="ordenador_listagem" ordenar="mundescricao__desc"/>
                        </td>
                    </tr>
                    <tr>
                        <th class="center" >
                            <img width="20px" class="img_filtro_listagem" src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_obrid_construcao" value="<?php echo $_REQUEST['filtro_obrid_construcao']; ?>" />
                        </th>
                        <th class="center" >
                            <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_obrnome_construcao" value="<?php echo $_REQUEST['filtro_obrnome_construcao']; ?>" />
                        </th>
                        <th class="center" >
                            <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_estuf_construcao" value="<?php echo $_REQUEST['filtro_estuf_construcao']; ?>" />
                        </th>
                        <th class="center" >
                            <img width="20px" class="img_filtro_listagem"  src="../imagens/icones/icons/busca.png" style="vertical-align:middle;"/>
                            <input type="text" class="filtro_listagem" name="filtro_mundescricao_construcao" value="<?php echo $_REQUEST['filtro_mundescricao_construcao']; ?>" />
                        </th>
                    </tr>
    				<?php
    				$totalObras=0;
    				if($obras['Construção']){
    					foreach($obras['Construção'] as $count => $dado):
    					?>
    					<tr <?php echo ($count%2) ? 'class="zebrado"' : ''; ?>>
    						<td class="" width="20%">
    							<?php
    							if($_REQUEST['tipo']==1){?>
    								<img class="link" src="cockpit/images/icone_o.png" style="vertical-align:middle;" width="20" height="20" title="Obras" onclick="abreObras(<?=$dado['obrid']?>);" />
    							<?php
    							}else{
								?>
									<img class="link" src="cockpit/images/icone_o.png" style="vertical-align:middle;" width="20" height="20" title="Obras" onclick="abreObras1(<?=$dado['obrid']?>);" />
								<?php
								}
    							?>

    							<?php echo $dado['obrid'] ?>
    						</td>
    						<td class="" width="45%"><?php echo $dado['obrnome'] ?></td>
    						<td class="center" width="15%"><?php echo $dado['estuf'] ?></td>
    						<td class="" width="20%"><?php echo $dado['mundescricao'] ?></td>
    					</tr>
    					<?php
    					$totalObras+=1;
    					endforeach;
    						?>
    						<tr>
    							<th class="bold" colspan="3">Total de Obras</th>
    							<th class="bold numero" ><?php echo $totalObras ?></th>
    						</tr>
    					<?php
    					}else{
    					?>
    						<tr>
    							<td  class="center" style="color:red" colspan="5">Não existem obras nessa situação.</td>
    						</tr>
    					<?php
    					}
    					?>
    			</table>
    		</td>
    	</tr>
    </table>
</form>
</body>
</html>