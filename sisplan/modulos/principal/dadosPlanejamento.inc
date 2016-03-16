<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Expires" content="-1">
    <title> </title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/jquery.1.3.2.min.js"></script>
    <script src="../includes/jquery.tabs.js"></script>
    <script src="../includes/calendario.js"></script>
    <script src="../includes/micoxAjax.js"></script>


    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <style type="text/css">
		.principal { width:550px;}
    </style>
    <style type="text/css" media="print">
		.principal {width:100%;}
		.noPrint {display:none;}
    </style>
  </head>
  <body>
<div class='principal' style="float:left;">
<?php
monta_titulo( $titulo_modulo, '');


$arMes = array('1'  => 'Janeiro',
               '2'  => 'Fevereiro',
               '3'  => 'Março',
               '4'  => 'Abril',
               '5'  => 'Maio',
               '6'  => 'Junho',
               '7'  => 'Julho',
               '8'  => 'Agosto',
               '9'  => 'Setembro',
               '10' => 'Outubro',
               '11' => 'Novembro',
               '12' => 'Dezembro');

$projetoPlanejamento = new ProjetoPlanejamento($_REQUEST['pplid']);
$dadosOriginais		 = $projetoPlanejamento->getDados();

$where[] = "ppl.pplid = {$_REQUEST['pplid']}";
$rs = $projetoPlanejamento->listaByWhereXls($where, false);
$dadosPlanejamento = $rs[0];
?>
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Unidade Executora:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['unidade'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Tipo de Projeto:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['tipoprojeto'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Prioridade:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['pplprioridade'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Título:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['ppltitulo'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Objetivos:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['pplobjetivo'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Justificativa:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['ppljustificativa'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Descrição:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['ppldescricao'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Vinculação ao Planejamento Estratégico:
            </td>
            <td>
                <?php
						$sql = " select
									dfe.dfedsc || ' - ' || ite.itedsc as descricao
								from
									sisplan.iniciativaestrategica ite
								inner join
									sisplan.projetoiniciativa pin on pin.iteid = ite.iteid
								inner join
									sisplan.desafioestrategico dfe on dfe.dfeid = ite.dfeid
								where
									ite.itestatus = 'A' and
									pin.pplid = '" . (int) $_REQUEST['pplid'] . "'
								order by
									ite.iteid";

						$vinculacaoPlanejamento = $db->carregarColuna( $sql );
						$vinculacaoPlanejamento = $vinculacaoPlanejamento ? $vinculacaoPlanejamento : array();

						foreach( $vinculacaoPlanejamento as $dado){
							echo $dado."<br>";
						}
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Otras Iniciativas:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['pploutrasiniciativas'];
                ?>
            </td>
        </tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Pac:</td>
		</tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Ações presentes nos APPCs:
            </td>
            <td>
                <?php
					echo $dadosOriginais['ploflgappcs'] == 't' ? 'Sim' : 'Não';
                ?>
            </td>
        </tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Regionalização:</td>
		</tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Localização da ação:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['esfdsc'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 País:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['paidescricao'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Unidade Federativa:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['unidadefederativa'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Município:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['mundescricao'];
                ?>
            </td>
        </tr>
        <tr>
        <td colspan="2">
	        <table width="90%" id="tableFisico" align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
				<tr>
	                <td class="SubTituloDireita" style="text-align: center; font-weight: bold;" colspan="3">
	                    <center>Abrangência</center>
	                </td>
	            </tr>
	            <tr>
	                <td class="SubTituloDireita" style="text-align: center; font-weight: bold;">
	                    País
	                </td>
	                <td class="SubTituloDireita" style="text-align: center; font-weight: bold;">
	                    Unidade Federativa
	                </td>
	                <td class="SubTituloDireita" style="text-align: center; font-weight: bold;">
	                    Município
	                </td>
	            </tr>

	        <?php
	          //Carregando Abrangência
			  $sql = "
					SELECT
						p.paidescricao AS pais,
						e.estuf||' - '||e.estdescricao AS uf,
						CASE r.estuf
							WHEN 'DF' THEN ra.rgadsc
							ELSE m.estuf||' - '||m.mundescricao
						END AS mun
					FROM
						sisplan.projetoregionalizacao r
						JOIN territorios.pais p
							ON p.paiid = r.paiid
						LEFT JOIN territorios.municipio m
							ON m.muncod = r.muncod
						LEFT JOIN territorios.estado e
							ON e.estuf = r.estuf
						LEFT JOIN planointerno.regiaoadministrativa ra
							ON ra.rgaid = r.rgaid
					WHERE
						r.pplid = ".(int) $_REQUEST['pplid'];

			    $dados = $db->carregar($sql);
			    $dados = is_array($dados) ? $dados : array();

				if ( !empty($dados) ){

						foreach ($dados as $dado){
						?>
							<tr>
								<td><?php echo $dado['pais']; ?></td>
								<td><?php echo $dado['uf']; ?></td>
								<td><?php echo $dado['mun']; ?></td>
							</tr>
						<?php
						}
				}else{
						?>
							<tr>
								<td colspan="3" ><center>Nenhuma Abrangência cadastrada</center></td>
							</tr>
						<?php
				}

	        ?>
		</table>
		</td>
		</tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">PPA:</td>
		</tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Programa:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['programa'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Ação:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['acao'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Unidade Orçamentária:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['uni'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Localizador:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['localizador'];
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Plano Orçamentário:
            </td>
            <td>
                <?php
					echo $dadosPlanejamento['plonome'];
                ?>
            </td>
        </tr>
		<tr>
		<td colspan="2">
		        <table width="90%" id="tableFisico" align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
					<tr>
		                <td class="SubTituloDireita" style="text-align: center; font-weight: bold;" colspan="3">
		                    <center>Produto Secundário</center>
		                </td>
		            </tr>
		            <tr>
		                <td class="SubTituloDireita" style="text-align: center; font-weight: bold;">
		                    Produto
		                </td>
		                <td class="SubTituloDireita" style="text-align: center; font-weight: bold;">
		                    Unidade Medida
		                </td>
		                <td class="SubTituloDireita" style="text-align: center; font-weight: bold;">
		                    Meta
		                </td>
		            </tr>
					<tr>
						<td><?php echo $dadosPlanejamento['pdudsc']; ?></td>
						<td><?php echo $dadosPlanejamento['udadsc']; ?></td>
						<td><?php echo $dadosPlanejamento['pplprosecundariometa']; ?></td>
					</tr>
			</table>
		</td>
		</tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Contratações:</td>
		</tr>
        <tr>
        <td colspan="2">
	        <table width="90%" id="tableFisico" align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
	            <tr>
	                <td class="SubTituloDireita" style="text-align: center; font-weight: bold;">
	                    Forma da contratação
	                </td>
	                <td class="SubTituloDireita" style="text-align: center; font-weight: bold;">
	                    Objeto contratado
	                </td>
	                <td class="SubTituloDireita" style="text-align: center; font-weight: bold;">
	                    Valor Custeio
	                </td>
	                <td class="SubTituloDireita" style="text-align: center; font-weight: bold;">
	                    Valor Capital
	                </td>
	            </tr>

	        <?php
			//Carregando Contratações
            $modelo = new ProjetoContratacao();
			$dados  = $modelo->getListaContratacoesByPplid( (int) $_REQUEST['pplid']);

			$dados = is_array($dados) ? $dados : array();

				if ( !empty($dados) ){

						foreach ($dados as $dado){
						?>
							<tr>
								<td><?php echo $dado['forma_dsc']; ?></td>
								<td><?php echo $dado['objeto']; ?></td>
								<td><?php echo formata_valor($dado['valorcusteio']); ?></td>
								<td><?php echo formata_valor($dado['valorcapital']); ?></td>
							</tr>
						<?php
						}
				}else{
						?>
							<tr>
								<td colspan="4" ><center>Nenhuma Contratação cadastrada</center></td>
							</tr>
						<?php
				}

	        ?>
		</table>
		</td>
		</tr>
		<tr>
			<td colspan="7" class="SubTituloCentro" style="background-color: #DCDCDC">Cronogramas:</td>
		</tr>
		<tr>
			<td colspan="2">
			 <table cellspacing="0" cellpadding="0" >
                        <tr align="center">
                            <td>
								<center>
                                <table id="tableFisico" align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 280px; float:left; margin-bottom: 10px; ">
                                    <tr>
                                        <td class="SubTituloDireita" style="text-align: center; font-weight: bold;" colspan="3">
                                            Cronograma Físico
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" style="text-align: center;">
                                            &nbsp;
                                        </td>
                                        <td class="SubTituloDireita" style="text-align: center;">
                                            Previsto
                                        </td>
                                    </tr>
                                    <?php foreach ($arMes as $numMes => $nomeMes) :?>
                                    <tr>
                                        <td align='right' class="SubTituloDireita">
                                            <?= "{$nomeMes}/{$_SESSION['exercicio']}"; ?>
                                        </td>
                                        <td>
                                            <?php echo (int) $dadosPlanejamento["crfprevistomes{$numMes}"]; ?>
                                        </td>
                                    </tr>
                                    <?php endforeach;?>
                                </table>
								</center>
                            </td>
                        </tr>
                        <tr>
                            <td>

                                <table id="tableOrcamentario" align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 500px; float: left; margin-bottom: 10px; <?= $style; ?>">
                                    <tr>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center; font-weight: bold;" colspan="3">
                                            Cronograma Orçamentário
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            &nbsp;
                                        </td>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            Previsto Custeio
                                        </td>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            Previsto Capital
                                        </td>
                                    </tr>
                                    <?php foreach ($arMes as $numMes => $nomeMes) :?>
                                    <tr>
                                        <td align='right' class="SubTituloDireita">
                                            <?= "{$nomeMes}/{$_SESSION['exercicio']}"; ?>
                                        </td>
                                        <td>
                                        	<?php echo formata_valor( $dadosPlanejamento["croprevistocusteiomes{$numMes}"] ); ?>
                                        </td>
                                        <td>
                                        	<?php echo formata_valor( $dadosPlanejamento["croprevistocapitalmes{$numMes}"] ); ?>
                                        </td>
                                    </tr>
                                    <?php endforeach; ?>
                                </table>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <table id="tableFinanceiro" align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 500px; float: left; margin-bottom: 10px; ">
                                    <tr>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center; font-weight: bold;" colspan="3">
                                            Cronograma Financeiro
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            &nbsp;
                                        </td>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            Previsto Custeio
                                        </td>
                                        <td class="SubTituloDireita" style="width:100px; text-align: center;">
                                            Previsto Capital
                                        </td>
                                    </tr>
                                    <?php foreach ($arMes as $numMes => $nomeMes) :?>
                                    <tr>
                                        <td align='right' class="SubTituloDireita">
                                            <?= "{$nomeMes}/{$_SESSION['exercicio']}"; ?>
                                        </td>
                                        <td>
                                        	<?php echo formata_valor( $dadosPlanejamento["cfiprevistocusteiomes{$numMes}"] ); ?>
                                        </td>
                                        <td>
                                        	<?php echo formata_valor( $dadosPlanejamento["cfiprevistocapitalmes{$numMes}"] ); ?>
                                        </td>
                                    </tr>
                                    <?php endforeach;?>
                                </table>
                            </td>
                        </tr>
                    </table>
			</td>
		</tr>
</table>
<br><br>
	</div>
    <div class='noPrint' style="vertical-align:top; float:left; width:20px;">
    	<br><br>
		<input class="botao" style="vertical-align:top;" type="button" value="Imprimir" onClick="window.print();"><br><br>
		<?php
			wf_desenhaBarraNavegacao( $dadosOriginais['docid'], array("pplid" => $_REQUEST['pplid']) );
		?>
	</div>
  </body>
</html>

