<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
	<body>
		<center>
			<?if($_REQUEST['id'] == 'EsporteLazer'){?>
			
				<h3>Diretor, você aderiu ao Esporte na Escola!!!</h3>
				<br />
				<div style="margin: 0 auto; padding: 0; height: 460px; width: 100%; border: none;" class="div_rolagem">
				
					<p style="text-align:justify;">
						O Ministério do Esporte parabeniza essa instituição pela iniciativa de participar desse Programa que incentiva a prática esportiva, integrando política esportiva educacional com a política de educação.
						<br>O Esporte na Escola é uma parceria entre o Ministério do Esporte e o Ministério da Educação, para promover o acesso ao esporte às crianças e aos jovens das escolas participantes do Mais Educação, da rede de ensino público brasileira. 
						<br>O Esporte na Escola será desenvolvido no Macrocampo Esporte e Lazer do Mais Educação, de acordo com a proposta pedagógica do esporte educacional, a qual proporcionará aos alunos múltiplas vivências esportivas. 
						<br>O responsável pelo desenvolvimento das atividades será o monitor, obrigatoriamente da área de Educação Física/Esporte (graduado ou graduando), selecionado pela Escola para atuar nesse Macrocampo. 
						</p>
					<br />
					<p style="text-align:justify;"><b>Principais benefícios:</b></p>
					
					<p style="text-align:justify;">	
						• Acesso às diversas modalidades coletivas e individuais, por meio do desenvolvimento de múltiplas vivências esportivas;<br />
						• Kit de materiais esportivos diversificados;<br />
						• Kit esportivo para modalidade Atletismo: Conforme informado no momento da adesão, nesta edição, a modalidade ATLETISMO terá uma atenção especial, inclusive com a destinação de kit esportivo e módulo de capacitação específico;<br>
						• Materiais didáticos (Livro de Fundamentos do PST e Cadernos de Apoio Pedagógico – CAP);<br>
						• Capacitação para os professores/monitores, necessitando que a escola garanta a inscrição dos envolvidos;<br>
						• Acompanhamento pedagógico realizado por profissionais vinculados às Instituições de Ensino Superior da região.
					</p>
					<br />
					<p style="text-align:justify;">Para viabilizar o acompanhamento das atividades na escola e a participação dos monitores na capacitação, <b>será necessário preencher o cadastro da escola, do diretor e do monitor no sistema do Segundo Tempo no sítio eletrônico: www.pst.uem.br.</b> Para acessar o sistema de cadastro é necessário:</p>
					<p style="text-align:justify;">	• Inserir o CPF do Diretor da escola como login;<br />
						• Digitar a senha 123456789: A senha deve ser modificada após o 1º acesso.
					</p>
					<br />
					<p style="text-align:justify;">	
						É importante destacar que somente o CPF do diretor cadastrado no <?php echo SIGLA_SISTEMA; ?> terá o acesso liberado, portanto é imprescindível que os dados cadastrais do diretor e da escola estejam atualizados no <?php echo SIGLA_SISTEMA; ?>. Assim que o MEC finalizar a relação dos diretores cadastrados, você receberá uma mensagem eletrônica para que a escola proceda ao cadastramento no Sistema do Segundo Tempo.
						Estamos à disposição para esclarecimentos por e-mail segundotempo_maisedu@esporte.com.br ou pelos telefones (61) 3217-9490/9691/1490/1465.
					</p>
					<br />
					<p style="font-size: 10px" align="right">
						<b>
							MINISTÉRIO DO ESPORTE<br>
							SECRETARIA NACIONAL DE ESPORTE, EDUCAÇÃO, LAZER E INCLUSÃO SOCIAL<br>
							DEPARTAMENTO DE DESENVOLVIMENTO E ACOMPANHAMENTO DE POLÍTICAS E PROGRAMAS INTERSETORIAS<br>
							PROGRAMA SEGUNDO TEMPO
						</b>
					</p>
					
				</div>
				<br>
				<input type="button" value="FECHAR" onclick="closeMessage();" />
				
			<?}elseif($_REQUEST['id'] == 'PromocaoSaude'){?>
			
				<h3>VENHA PARA ESSE MOVIMENTO !</h3>
				<br />
				<div style="margin: 0 auto; padding: 0; height: 460px; width: 100%; border: none;" class="div_rolagem">
				
					<p style="text-align:justify;">
						Agora no Macrocampo Promoção à Saúde, sua escola poderá desenvolver múltiplas vivências e ações promotoras de saúde, integradas em uma única proposta pedagógica.
						<br>Em 2013 o Macrocampo  Promoção à Saúde teve uma atenção especial com a articulação do Programa Saúde na Escola- PSE.  
						</p>
					<br />
					<p style="text-align:justify;"><b>NOVIDADES:</b></p>
					<br />
					<p style="text-align:justify;">As ações desenvolvidas pelas escolas no Macrocampo Promoção da Saúde, contará para o alcance de metas do Programa Saúde na Escola (PSE).</p>
					<br />
					<p style="text-align:justify;">	
						<b>Aderindo ao Macrocampo</b> a escola receberá recursos de custeio e capital para aquisição de materiais específicos para o desenvolvimento destas ações articuladas aos demais macrocampos e atividades selecionadas pela escola.
					</p>
					<br />
					<p style="text-align:justify;">
						<b>
							Maiores informações entre <a href="http://www.saúde.gov.br/pse" target="_blank">http://www.saúde.gov.br/pse</a>
						</b>
					</p>
					
				</div>
				<br>
				<input type="button" value="FECHAR" onclick="closeMessage();" />
				
			<?}?>
			<!-- 
			<input type="button" value="Aderir ao Segunto Tempo" onclick="aderirPST('meajax.php', 'tipo=aderir_pst'); closeMessage();" />
			&nbsp;&nbsp;&nbsp;
			<input type="button" value="Não Aderir ao Segunto Tempo" onclick="naoAderirPST('meajax.php', 'tipo=nao_aderir_pst'); closeMessage();" />
			 -->
		</center>
	</body>
</html>