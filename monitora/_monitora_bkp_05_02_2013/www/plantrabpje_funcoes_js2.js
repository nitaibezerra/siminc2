function altera_pto(cod) {
	document.formulario.act2.value = 'alterar';
	document.formulario.ptoid.value = cod;
	document.formulario.submit();
}
function cancela() {
	document.formulario.ptoid.value = '';
	document.formulario.submit();
}

function excluir_pto(cod,dsc) {

	if( window.confirm( "Confirma a exclusão da Atividade "+dsc+" ?") )
	{
		document.formulario.exclui.value = cod;
		document.formulario.submit();
	} else document.formulario.exclui.value = 0;

}

function aprova_ativ(cod) {
	document.formulario.ptoid.value = cod;
	document.formulario.act.value = 'aprov';
	document.formulario.submit();

}




function aprova_retorno(cod) {
	document.formulario.ptoid.value = cod;
	document.formulario.act.value = 'retorno';
	document.formulario.submit();

}

function insere_pt(cod) {
	document.formulario.act2.value = cod;
	document.formulario.ptotipo.value = cod;
	document.formulario.ptoid.value = '';
	document.formulario.submit();
	
	// e = "monitora.php?modulo=principal/projespec/plantrabpje_insercao&acao=I&act2="+cod+"&ptotipo="+cod;
     //window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=800,height=600'");
	
	

}
function chama_macroetapa(cod) {
	document.formulario.act2.value = cod;
	document.formulario.submit();

}

function validar_cadastro(cod)
{
	altdata = false;
	altproj = false;
	altpai = false;
	//if (!validaBranco(document.formulario.ptocod, 'Código')) return;

	//if ( trim(document.formulario.ptocod.value.length)< 4 )
	//{
		//alert ('O Código tem que ter um mínimo de 4 caracteres!');
		//document.formulario.ptocod.focus();
		//return;
	//}
	if (!validaBranco(document.formulario.ptodsc, 'Título')) return;
	if (!validaData(document.formulario.ptodata_ini))
	{
		alert("Data Inicio Inválida.");
		document.formulario.ptodata_ini.focus();
		return;
	}

	if (!validaDataMaior(document.formulario.ptodata_ini , document.formulario.ptodata_fim))
	{
		alert("Data Término não pode ser Anterior que Data Início.");
		document.formulario.ptodata_fim.focus();
		return;
	}
	//data existe e é válida, nesta caso tem de existir uma data fim
	if (!validaBranco(document.formulario.ptodata_fim, 'Data Término')) return;
	// a data fim exite. verifico se é válida
	if (!validaData(document.formulario.ptodata_fim))
	{
		alert("Data Término Inválida.");
		document.formulario.ptodata_fim.focus();
		return;
	}
	// a data fim é válida. Tenho que verificar se ela é menor ou igual a de Início.
	if (!validaDataMaior(document.formulario.ptodata_ini , document.formulario.ptodata_fim))
	{
		alert("Data Término não pode ser anterior que Data Início.");
		document.formulario.ptodata_fim.focus();
		return;
	}

	if (!validaBranco(document.formulario.unmcod, 'Unidade de Medida!')) return;
	if (!validaBranco(document.formulario.ptoprevistoexercicio, 'Previsto no Exercï¿½cio!')) return;
	
	//Verificaï¿½ï¿½o dos valores de desembolso
	if( !VerificaSaldo( document.formulario.ptoid_pai.value ) ) return;
	
	selectAllOptions( document.getElementById( 'ptovlrprevisto' ) );
	
	for(var i = 0 ; i < document.getElementById( 'ptovlrprevisto' ).options.length ; i++ )
	{
		elm = document.getElementById( 'ptovlrprevisto' ).options[ i ];
		auxData = new Object();
		auxData.value = elm.value.split( ' - ' )[ 0 ]
		if( !validaDataMaior( document.formulario.dtini, auxData ) )
		{
			alert( 'Existe uma previsão de desembolso anterior ao Início da atividade' );
			return;
		}
	}
	
	// passou por todos os testes entï¿½o devo verificar as datas

	if ( !validaDataMaior(document.formulario.dtini , document.formulario.ptodata_ini))
	{
		altproj=true;
		altdata=true;
	}
	if ( !validaDataMaior(document.formulario.ptodata_fim , document.formulario.dtfim))
	{
		altproj=true;
		altdata=true;
	}

	if( altdata == true || altproj == true )
	{
		if (document.formulario.projfechado.value=='t' && altproj==true)
		{
			alert (' O Projeto está com suas datas congeladas e não pode ser alterado!\n Corrija as datas antes de prosseguir!');
			return;
		}
	}
	var auxDataIni = new Object();
	var auxDataFim = new Object();

	if( document.formulario.ptoordem_antecessor.value != "" )
	{
		for( var i = 0; i < arrAntecessores.length ; i++ )
		{

			if( arrAntecessores[ i ][ 'ptoordem' ] == document.formulario.ptoordem_antecessor.value )
			{
				if( arrAntecessores[ i ][ 'ptoid' ] == document.formulario.ptoid_pai.value )
				{
					alert( 'A atividade antecessora não pode ser a Macro-Etapa agregadora' );
					return;
				}
				auxDataIni.value = arrAntecessores[ i ][ 'dataini' ];
				auxDataFim.value = arrAntecessores[ i ][ 'datafim' ];
				if( !validaDataMaior( auxDataFim, document.formulario.ptodata_ini )  )
				{
					alert( 'Não é possível alterar as datas da atividade antecessora.\nCorrija as datas antes de prosseguir.' );
					return;
				}
			}
		}
	}
	if( document.formulario.ptoid_pai.value != "" )
	{
		for( var i = 0; i < arrMacroEtapas.length ; i++ )
		{
			if( arrMacroEtapas[ i ][ 'ptoid' ] == document.formulario.ptoid_pai.value )
			{
				auxDataIni.value = arrMacroEtapas[ i ][ 'dataini' ];
				auxDataFim.value = arrMacroEtapas[ i ][ 'datafim' ];
				if( !validaDataMaior( auxDataIni, document.formulario.ptodata_ini ) || !validaDataMaior( document.formulario.ptodata_fim, auxDataFim ) )
				{
					altpai = true;
					if( arrMacroEtapas[ i ][ 'datafechada' ] == 't' )
					{
						alert( 'A Macro-Etapa agregadora está com suas datas congeladas e nãopode ser alterada!\n Corrija as datas antes de prosseguir!' );
						return;
					}
				}
			}
		}
	}
	if( altdata == true || altproj == true || altpai == true )
	{
		if( window.confirm( "As datas inseridas irão alterar o período da Macro-Etapa e/ou do Projeto.\n Escolha OK para confirmar ou Cancelar para corrigir!") )
		{
			if (cod == 'I') document.formulario.act.value = 'inserir';
			else document.formulario.act.value = 'alterar';
			document.formulario.submit();
		}
	}
	else
	{
		if (cod == 'I') document.formulario.act.value = 'inserir';
		else document.formulario.act.value = 'alterar';

		document.formulario.submit();
	}




}

var saldo;
var disponivel = 0;
function combo_desembolso_abre_janela( nome, w, h  )
{
	formulario = document.formulario;
	VerificaSaldo( formulario.ptoid_pai.value );
	elmCombo = document.getElementById( 'ptovlrprevisto' );
	totalRegistros = elmCombo.options[ 0 ].value == '' ? 0 : elmCombo.options.length;
	auxSaldo = saldo == 0 ? disponivel : saldo;
	janela = window.open( '../geral/combo_desembolso.php?nome=' + nome + '&saldo=' + auxSaldo + '&totalRegistros=' + totalRegistros , nome, "height=" + h +  ",width=" + w +  ",scrollbars=yes,top=50,left=200" );
}
function VerificaSaldo( ptoid )
{
	if( ptoid )
	{
		for( var i = 0 ; i < arrMacroEtapas.length ; i++ )
		{
			if( arrMacroEtapas[ i ][ 'ptoid' ] == ptoid )
			{
				previsao = retornaPrevisao();
				previsaoPai = arrMacroEtapas[ i ][ 'somaDespesa' ];
				saldo = previsaoPai;
				if( saldo < 0 )
				{
					alert( 'A previsão de desembolso supera a previsão da Macro-Etapa agregadora.' );
					return false;
				}
			}
		}
	}
	else
	{
		previsao = retornaPrevisao();
		saldo = limiteProjeto;
		if( saldo < 0 )
		{
			alert( 'A previsão de desembolso supera o valor do Projeto Especial.' );
			return false;
		}
	}
	return true;
}
function retornaPrevisao()
{
	elmCombo = document.getElementById( 'ptovlrprevisto' );
	totalElm = elmCombo.options[ 0 ].value == '' ? 0 : elmCombo.options.length;
	previsao = 0;
	for( var i = 0 ; i < totalElm ; i++ )
	{
		elm = elmCombo.options[ i ];
		if( elm.value != '' )
		{
			valorAux = parseInt( elm.value.split( ' - ' )[ 1 ] );
			previsao += valorAux;
		}
	}
	return previsao;
}

/**
* Abre popup dos arquivos relacionados ao projeto especial de acordo com a acao (L - listar, I - inserir)
* 
* @param string acao
*/
function popup_arquivo( acao, nome, width, height )
{
	window.open( '../geral/popup_arquivo.php?acao=' + acao + '&nome=' + nome, '', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
}


function exibe_grafico(cod)
{
	rbn=1000;
     	if (formulario.rbnivel[0].checked) rbn=1
  	else if (formulario.rbnivel[1].checked) rbn=2
  	else if (formulario.rbnivel[2].checked) rbn=3  
  	else if (formulario.rbnivel[3].checked) rbn=1000;
  	
  	
  	if (! cod) cod=0;
  	e="../geral/gantt.php?ptoid="+cod+"&nivel="+rbn;
  	
    window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes");  	
	
	//window.open( "gantt', "menubar=yes,location=yes,resizable=yes,scrollbars=yes,status=yes" );
	
	
}

function abrir_arvore(cod)
{
	document.formulario.abrirarvore.value = cod;
	document.formulario.submit();
	
}
function submete_aprov()
{
	document.formulario.act.value = 'aprovalote';
	document.formulario.submit();
}
