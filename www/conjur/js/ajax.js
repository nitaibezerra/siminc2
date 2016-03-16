//function listar_procedencia( unpid )
//{
//    var div_on = document.getElementById( 'proid_on' );
//	var div_off = document.getElementById( 'proid_off' );        
//	var prodsc = document.getElementById( 'prodsc' ); 
//	div_on.style.display = 'block';
//	div_off.style.display = 'none';
//	if(unpid){
//		if(!prodsc.disabled){
//		}
//		 return new Ajax.Updater(div_on, 'ajax.php',
//			     {     
//			        method: 'post',
//			        parameters: '&servico=listar_proc&unpid=' + unpid,
//			        onComplete: function(res)
//			        {	
//			         //alert(res.responseText);
//			 		//	atualiza_proc();  
//			        }
//			    });
//	}else{
//		prodsc.value = '';
//		div_on.style.display = 'none';
//		div_off.style.display = 'block';
//	}
//}

function listar_procedencia( unpid , proid )
{
    var div_on = document.getElementById( 'proid_on' );
	var div_off = document.getElementById( 'proid_off' );        
	div_on.style.display = 'block';
	div_off.style.display = 'none';
	if(unpid){
		 return new Ajax.Updater(div_on, 'ajax.php',
			     {     
			        method: 'post',
			        parameters: '&servico=listar_proc&unpid=' + unpid + '&proid=' + proid,
			        onComplete: function(res)
			        {	
			         //alert(res.responseText);			 		
			        }
			    });
	}else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
	}
}

