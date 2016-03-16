
function escolaAtivaAbreInstrumento( inuid, tipo ){
	
	window.location.href = "escolaativa.php?modulo=principal/programa&acao=A&inuid=" + inuid + "&tipo=" + tipo;
	
}

function escolaAtivaAbreSecretario( entid ){
	
	window.open('?modulo=principal/escolaAtivaNovaContato&acao=A&entid=' + entid, 'Dirigentes', 'scrollbars=yes, height=600, width=700, status=no, toolbar=no, menubar=no,location=no');
	
}

function escolaAtivaExcluirEscola( id ){
	
	if ( confirm("Deseja realmente excluir esta escola?") ){
		
		var linha  = document.getElementById( "escola_" + id );
		var tabela = document.getElementById( "listaEscolas" );
	 	tabela.deleteRow( linha.rowIndex );
		
	}
	
}

function escolaAtivaIncluiEscola(){
	
	var tabela  = document.getElementById( "listaEscolas" );
	var tamanho = tabela.rows.length;
	var escola  = document.getElementById( "entid" );
	
	if( escola.value == "" ){
		alert( "Favor selecionar uma escola!" );
		return false;
	}else{
	
		var escolaExistente = document.getElementById( "escola_" + escola.value );
		
		if ( escolaExistente != null ){
			alert( "Esta escola já está inserida no Programa!" );
			escola.value = "";
			return false;
		}else{
			
			// pega os valores e dados necessários
			var posicao = escola.options[escola.selectedIndex].text.indexOf("-");
			posicao 	= Number(posicao + 1);
			
			var nome = escola.options[escola.selectedIndex].text.slice(posicao);
			
			var eaeqtdpredios = document.getElementById( "eaeqtdpredios" );
			var eaeqtdturmas  = document.getElementById( "eaeqtdturmas" );
			var eaeqtdalunos1 = document.getElementById( "eaeqtdalunos1" );
			var eaeqtdalunos2 = document.getElementById( "eaeqtdalunos2" );
			var eaeqtdalunos3 = document.getElementById( "eaeqtdalunos3" );
			var eaeqtdalunos4 = document.getElementById( "eaeqtdalunos4" );
			var eaeqtdalunos5 = document.getElementById( "eaeqtdalunos5" );
			var totalalunos   = document.getElementById( "totalAlunos" );
			
			// cria a tr
			var linha = tabela.insertRow(tamanho);
			linha.id  = "escola_" + escola.value;
			
			// colore a tr de acordo com a ultima da tabela
			if ( tamanho ){
				if( tabela.rows[tamanho-1].style.backgroundColor == "rgb(224, 224, 224)" ){
					linha.style.backgroundColor = "#f4f4f4";					
				}else{
					linha.style.backgroundColor = "#e0e0e0";					
				}
			}else{
				linha.style.backgroundColor = "#e0e0e0";
			}
			
			// cria as tds
			var colNome   = linha.insertCell(0);
			var colPredio = linha.insertCell(1);
			var colTurma  = linha.insertCell(2);
			var colAluno1 = linha.insertCell(3);
			var colAluno2 = linha.insertCell(4);
			var colAluno3 = linha.insertCell(5);
			var colAluno4 = linha.insertCell(6);
			var colAluno5 = linha.insertCell(7);
			var colTotal  = linha.insertCell(8);
			var colExclui = linha.insertCell(9);
			
			colNome.colSpan = "2";
			
			colNome.style.width   = "30%";
			colPredio.style.width = "8%";
			colTurma.style.width  = "8%";
			colAluno1.style.width = "5%";
			colAluno2.style.width = "5%";
			colAluno3.style.width = "5%";
			colAluno4.style.width = "5%";
			colAluno5.style.width = "5%";
			colTotal.style.width  = "5%";
			colExclui.style.width = "4%";
			
			colPredio.style.textAlign = "center";
			colTurma.style.textAlign  = "center";
			colAluno1.style.textAlign = "center";
			colAluno2.style.textAlign = "center";
			colAluno3.style.textAlign = "center";
			colAluno4.style.textAlign = "center";
			colAluno5.style.textAlign = "center";
			colTotal.style.textAlign  = "center";
			colExclui.style.textAlign = "center";
			
			// insere os valores de cada td
			colNome.innerHTML 	= nome;
			
			colPredio.innerHTML = "<input type='text' style='width: 7ex;' id='eaeqtdpredios[" + escola.value + "]' onblur='MouseBlur(this);escolaAtivaAtualizaTotal(\"predios\");' onmouseout='MouseOut(this);'" +
								  "onfocus='MouseClick(this);this.select();' onmouseover='MouseOver(this);' class='normal' onkeyup='this.value=mascaraglobal(\"####\",this.value);' " +
								  "value='" + eaeqtdpredios.value + "' maxlength='4' size='5' name='eaeqtdpredios[" + escola.value + "]'>";
			
			colTurma.innerHTML  = "<input type='text' style='width: 7ex;' id='eaeqtdturmas[" + escola.value + "]' onblur='MouseBlur(this);escolaAtivaAtualizaTotal(\"turmas\");' onmouseout='MouseOut(this);'" +
								  "onfocus='MouseClick(this);this.select();' onmouseover='MouseOver(this);' class='normal' onkeyup='this.value=mascaraglobal(\"####\",this.value);' " +
								  "value='" + eaeqtdturmas.value + "' maxlength='4' size='5' name='eaeqtdturmas[" + escola.value + "]'>";
			
			colAluno1.innerHTML = "<input type='text' style='width: 7ex;' id='eaeqtdalunos1[" + escola.value + "]' onblur='MouseBlur(this);escolaAtivaAtualizaTotal(\"alunos1\"); escolaAtivaAtualizaTotalAlunos();' onmouseout='MouseOut(this);'" +
								  "onfocus='MouseClick(this);this.select();' onmouseover='MouseOver(this);' class='normal' onkeyup='this.value=mascaraglobal(\"####\",this.value);' " +
								  "value='" + eaeqtdalunos1.value + "' maxlength='4' size='5' name='eaeqtdalunos1[" + escola.value + "]'>";
			
			colAluno2.innerHTML = "<input type='text' style='width: 7ex;' id='eaeqtdalunos2[" + escola.value + "]' onblur='MouseBlur(this);escolaAtivaAtualizaTotal(\"alunos2\"); escolaAtivaAtualizaTotalAlunos();' onmouseout='MouseOut(this);'" +
							      "onfocus='MouseClick(this);this.select();' onmouseover='MouseOver(this);' class='normal' onkeyup='this.value=mascaraglobal(\"####\",this.value);' " +
							      "value='" + eaeqtdalunos2.value + "' maxlength='4' size='5' name='eaeqtdalunos2[" + escola.value + "]'>";
			
			colAluno3.innerHTML = "<input type='text' style='width: 7ex;' id='eaeqtdalunos3[" + escola.value + "]' onblur=MouseBlur(this);escolaAtivaAtualizaTotal(\"alunos3\"); escolaAtivaAtualizaTotalAlunos();' onmouseout='MouseOut(this);'" +
							      "onfocus='MouseClick(this);this.select();' onmouseover='MouseOver(this);' class='normal' onkeyup='this.value=mascaraglobal(\"####\",this.value);' " +
							      "value='" + eaeqtdalunos3.value + "' maxlength='4' size='5' name='eaeqtdalunos3[" + escola.value + "]'>";
			
			colAluno4.innerHTML = "<input type='text' style='width: 7ex;' id='eaeqtdalunos4[" + escola.value + "]' onblur='MouseBlur(this);escolaAtivaAtualizaTotal(\"alunos4\"); escolaAtivaAtualizaTotalAlunos();' onmouseout='MouseOut(this);'" +
							      "onfocus='MouseClick(this);this.select();' onmouseover='MouseOver(this);' class='normal' onkeyup='this.value=mascaraglobal(\"####\",this.value);' " +
							      "value='" + eaeqtdalunos4.value + "' maxlength='4' size='5' name='eaeqtdalunos4[" + escola.value + "]'>";
			
			colAluno5.innerHTML = "<input type='text' style='width: 7ex;' id='eaeqtdalunos5[" + escola.value + "]' onblur='MouseBlur(this);escolaAtivaAtualizaTotal(\"alunos5\"); escolaAtivaAtualizaTotalAlunos();' onmouseout='MouseOut(this);'" +
							      "onfocus='MouseClick(this);this.select();' onmouseover='MouseOver(this);' class='normal' onkeyup='this.value=mascaraglobal(\"####\",this.value);' " +
							      "value='" + eaeqtdalunos5.value + "' maxlength='4' size='5' name='eaeqtdalunos5[" + escola.value + "]'>";
			
			colTotal.innerHTML =  "<input type='text' style='width: 7ex;' id='totalalunos[" + escola.value + "]' onblur='MouseBlur(this);' onmouseout='MouseOut(this);'" +
							      "onfocus='MouseClick(this);this.select();' onmouseover='MouseOver(this);' class='disabled' onkeyup='this.value=mascaraglobal(\"####\",this.value);' " +
							      "value='" + totalalunos.value + "' maxlength='4' size='5' name='totalalunos[" + escola.value + "]' readonly='readonly>";
			
			colExclui.innerHTML = "<img src='../imagens/excluir.gif' title='Excluir Escola' onclick='escolaAtivaExcluirEscola(" + escola.value + ");' style='cursor:pointer;'/>";
		
			// limpa os campos
			escola.value		= "";
			eaeqtdpredios.value = "";
			eaeqtdturmas.value  = "";
			eaeqtdalunos1.value = "";
			eaeqtdalunos2.value = "";
			eaeqtdalunos3.value = "";
			eaeqtdalunos4.value = "";
			eaeqtdalunos5.value = "";
			totalalunos.value   = "";
		
		}
		
	}
	
}

function escolaAtivaLimpaEscola(){
	
	var escola  	  = document.getElementById( "entid" );
	var eaeqtdpredios = document.getElementById( "eaeqtdpredios" );
	var eaeqtdturmas  = document.getElementById( "eaeqtdturmas" );
	var eaeqtdalunos1 = document.getElementById( "eaeqtdalunos1" );
	var eaeqtdalunos2 = document.getElementById( "eaeqtdalunos2" );
	var eaeqtdalunos3 = document.getElementById( "eaeqtdalunos3" );
	var eaeqtdalunos4 = document.getElementById( "eaeqtdalunos4" );
	var eaeqtdalunos5 = document.getElementById( "eaeqtdalunos5" );
	var totalalunos   = document.getElementById( "totalAlunos" );
	
	escola.value		= "";
	eaeqtdpredios.value = "";
	eaeqtdturmas.value  = "";
	eaeqtdalunos1.value = "";
	eaeqtdalunos2.value = "";
	eaeqtdalunos3.value = "";
	eaeqtdalunos4.value = "";
	eaeqtdalunos5.value = "";
	totalalunos.value   = "";
	
}

function escolaAtivaAtualizaTotal( tipo ){
	
	var form = document.getElementById("formulario");
	var soma = "";
	
	var totaleaeqtdpredios = document.getElementById( "totaleaeqtdpredios" );
	var totaleaeqtdturmas  = document.getElementById( "totaleaeqtdturmas" );
	var totaleaeqtdalunos1 = document.getElementById( "totaleaeqtdalunos1" );
	var totaleaeqtdalunos2 = document.getElementById( "totaleaeqtdalunos2" );
	var totaleaeqtdalunos3 = document.getElementById( "totaleaeqtdalunos3" );
	var totaleaeqtdalunos4 = document.getElementById( "totaleaeqtdalunos4" );
	var totaleaeqtdalunos5 = document.getElementById( "totaleaeqtdalunos5" );
	
	switch( tipo ){
		
		case "predios":

			for ( var i = 0; i < form.length; i++ ) {
				if( form.elements[i].id.substr(0,14) == "eaeqtdpredios[" ){
					soma = Number(soma) + Number(form.elements[i].value);
				}
			}
			
			totaleaeqtdpredios.value = soma;
			
		break;
		case "turmas":
			
			for ( var i = 0; i < form.length; i++ ) {
				if( form.elements[i].id.substr(0,13) == "eaeqtdturmas[" ){
					soma = Number(soma) + Number(form.elements[i].value);
				}
			}
			
			totaleaeqtdturmas.value = soma;
			
		break;
		case "alunos1":
			
			for ( var i = 0; i < form.length; i++ ) {
				if( form.elements[i].id.substr(0,14) == "eaeqtdalunos1[" ){
					soma = Number(soma) + Number(form.elements[i].value);
				}
			}
			
			totaleaeqtdalunos1.value = soma;
			
		break;
		case "alunos2":
			
			for ( var i = 0; i < form.length; i++ ) {
				if( form.elements[i].id.substr(0,14) == "eaeqtdalunos2[" ){
					soma = Number(soma) + Number(form.elements[i].value);
				}
			}
			
			totaleaeqtdalunos2.value = soma;
			
		break;
		case "alunos3":
			
			for ( var i = 0; i < form.length; i++ ) {
				if( form.elements[i].id.substr(0,14) == "eaeqtdalunos3[" ){
					soma = Number(soma) + Number(form.elements[i].value);
				}
			}
			
			totaleaeqtdalunos3.value = soma;
			
		break;
		case "alunos4":
			
			for ( var i = 0; i < form.length; i++ ) {
				if( form.elements[i].id.substr(0,14) == "eaeqtdalunos4[" ){
					soma = Number(soma) + Number(form.elements[i].value);
				}
			}
			
			totaleaeqtdalunos4.value = soma;
			
		break;
		case "alunos5":
			
			for ( var i = 0; i < form.length; i++ ) {
				if( form.elements[i].id.substr(0,14) == "eaeqtdalunos5[" ){
					soma = Number(soma) + Number(form.elements[i].value);
				}
			}
			
			totaleaeqtdalunos5.value = soma;
			
		break;
	}
	
	
	
}

function escolaAtivaAtualizaTotalAlunos( id ){
	
	
	if( id == null ){
	
		var eaeqtdalunos1 = document.getElementById( "eaeqtdalunos1" );
		var eaeqtdalunos2 = document.getElementById( "eaeqtdalunos2" );
		var eaeqtdalunos3 = document.getElementById( "eaeqtdalunos3" );
		var eaeqtdalunos4 = document.getElementById( "eaeqtdalunos4" );
		var eaeqtdalunos5 = document.getElementById( "eaeqtdalunos5" );
		var total 		  = document.getElementById( "totalAlunos" );
		
		total.value = Number( eaeqtdalunos1.value) +
					  Number( eaeqtdalunos2.value) +
					  Number( eaeqtdalunos3.value) +
					  Number( eaeqtdalunos4.value) +
					  Number( eaeqtdalunos5.value);
		
	}else{
		
		var eaeqtdalunos1 = document.getElementById( "eaeqtdalunos1[" + id + "]" );
		var eaeqtdalunos2 = document.getElementById( "eaeqtdalunos2[" + id + "]" );
		var eaeqtdalunos3 = document.getElementById( "eaeqtdalunos3[" + id + "]" );
		var eaeqtdalunos4 = document.getElementById( "eaeqtdalunos4[" + id + "]" );
		var eaeqtdalunos5 = document.getElementById( "eaeqtdalunos5[" + id + "]" );
		var total 		  = document.getElementById( "totalAlunos[" + id + "]" );
		
		total.value = Number( eaeqtdalunos1.value) +
					  Number( eaeqtdalunos2.value) +
					  Number( eaeqtdalunos3.value) +
					  Number( eaeqtdalunos4.value) +
					  Number( eaeqtdalunos5.value);
		
	}
	
}

function escolaAtivaSalvaPrograma(){
	
	var tecnicos    = document.getElementById( "esaqtdtecnicos" );
	var professores = document.getElementById( "esaqtdprofessores" ); 
	var form		= document.getElementById( "formulario" );
	
	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;
	
	if( professores.value == "" ){
		mensagem += 'Nº de Professores \n';
		validacao = false;
	}
	
	if( !validacao ){
		alert(mensagem);
		return validacao;
	}else{
		form.submit();
	}
	
}

function escolaAtivaAderiPrograma( inusituacaoadesao ){
	
	window.location.href = window.location.href + "&inusituacaoadesao=" + inusituacaoadesao 
	
}